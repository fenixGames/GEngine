#include "geometry.h"
#include <math.h>
#include <GL/glut.h>
#ifdef DEBUG
#include <stdio.h>
#endif

#define POINT_PREC  180.0f
#define PI  M_PI

using namespace GEngine::Geometry;
/**
 * Constructor of the mesh.
 */
Mesh::Mesh(Vector vect, Point po)
{
	normal = Vector(vect);
	point = Point(po);
}

Mesh::Mesh(const Mesh& mesh)
{
	normal = Vector(mesh.normal);
	point = Point(mesh.point);
}

Mesh::Mesh(Point points[3])
{
	Matrix	calc(3, 3,
			1.0,						1.0,						1.0,
			points[1].x - points[0].x, 	points[1].y - points[0].y, 	points[1].z - points[0].z,
			points[2].x - points[0].x, 	points[2].y - points[0].y, 	points[2].z - points[0].z),
			adj;
	int idx;
    double mod = 1.0;

	point = Point(points[0]);
	normal = Vector(3, 0.0, 0.0, 0.0);

	for (idx = 0; idx < 3; idx++) {
		adj = calc.getAdjoint(0, idx);
		normal.setElement(idx, mod * adj.determinant());
        mod *= -1.0;
	}
    normal = normal * (1 / normal.mod());
}

/**
 * Calculates the intersection of three meshes and returns the point where
 * these meshes concur.
 *
 * @param	Mesh	m1	The first of the meshes.
 * @param	Mesh	m2	The second of the meshes.
 * @param	Mesh	m3	The third of the meshes.
 *
 * @return 	The point where the meshes concur or NULL if they do not concur.
 */
Point *
Mesh::intersection(Mesh m1, Mesh m2, Mesh m3)
{
	Matrix meshMat(3, 3,
			m1.normal.getElement(0), m1.normal.getElement(1), m1.normal.getElement(2),
			m2.normal.getElement(0), m2.normal.getElement(1), m2.normal.getElement(2),
			m3.normal.getElement(0), m3.normal.getElement(1), m3.normal.getElement(2)),
		   invert;
	Vector cM1, cM2, cM3, cMeshes(3, 0.0, 0.0, 0.0), result;

	/* Getting the vectors of the constant matrix. */
	cM1 = Vector(3, m1.point.x, m1.point.y, m1.point.z);
	cM2 = Vector(3, m2.point.x, m2.point.y, m2.point.z);
	cM3 = Vector(3, m3.point.x, m3.point.y, m3.point.z);

	cMeshes.setElement(0, m1.normal * cM1);
	cMeshes.setElement(1, m2.normal * cM2);
	cMeshes.setElement(2, m3.normal * cM3);

	/* Checking if the equation could be solved. */
	if (meshMat.determinant() == 0)
		return NULL;

	invert = meshMat.invert();
	result = (invert) * cMeshes;
	return new Point(result.getElement(0), result.getElement(1), result.getElement(2));
}

/**
 * Constructors of the polyhedron.
 */
Polyhedron::Polyhedron(MeshList meshes)
{
    MeshList meshList;
    MeshList::iterator iter;

    for (iter = meshes.begin(); iter != meshes.end(); iter++)
        meshList.push_back(*iter);

    mode = GL_TRIANGLE_STRIP;

    getPoints(&meshList);
}

Polyhedron::Polyhedron(Mesh ** mlist, unsigned int nmeshes)
{
    MeshList   meshes;
    unsigned int idx;

    pointList = NULL;

    /* Check the input. If list is NULL or the number of points is zero, return. */
    if (nmeshes == 0 || mlist == NULL)
        return;

    for (idx = 0; idx < nmeshes; idx++)
        meshes.push_back(mlist[idx]);

    mode = GL_TRIANGLE_STRIP;
    getPoints(&meshes);
}

Polyhedron::Polyhedron(PointList list)
{
    PointList::iterator iter;

    pointList = new PointList();

    /* Copying the list of vertices. */
    for (iter = list.begin(); iter != list.end(); iter++)
        pointList->push_back(new Point(*(*iter)));

    mode = GL_TRIANGLE_STRIP;
}

Polyhedron::Polyhedron(Point ** list, unsigned int npoints)
{
    unsigned int idx;

    pointList = new PointList();

    /* Inserting the list of vertices. */
    for (idx = 0; idx < npoints; idx++)
        pointList->push_back(new Point(*list[idx]));
    mode = GL_TRIANGLE_STRIP;
}
/**
 * Calculates the points where the meshes merge.
 * @param   MeshList    * meshList  The list of meshes whose points must be calculated.
 */
void
Polyhedron::getPoints(MeshList * meshList)
{
	MeshList::iterator	i1, i2, i3;
    PointList vertices; /* Set a Vertices list. */
	PointList::iterator vit;
	Point	* temp;
	unsigned int nedges;

    pointList = new PointList();

    /* Going through the list of meshes and calculating the vertices of the polyhedron. */
	for (i1 = meshList->begin(); i1 != meshList->end(); i1++) {
		i2 = i1;
		for (i2++; i2 != meshList->end(); i2++) {
			i3 = i2;
			for (i3++; i3 != meshList->end(); i3++) {
				temp = Mesh::intersection(*(*i1), *(*i2), *(*i3));
				if (temp != NULL)
					vertices.push_back(temp);
			}
		}
	}

	/* Caculating the number of edges of the figure. */
	nedges = meshList->size() + vertices.size() - 2;
	while (!vertices.empty()) {
		temp = vertices.front();
		pointList->push_back(new Point(temp->x, temp->y, temp->z));
		vertices.pop_front();
	}

 	nedges -= pointList->size();
	vit = pointList->begin();

	/* If there are not enough vertices on the printing list, add the first ones again. */
	while (nedges > 0) {
		temp = * vit;
		pointList->push_back(new Point(temp->x, temp->y, temp->z));
		vit++;
		nedges--;
	}
}

/**
 * Destructor of the polyhedron.
 */
Polyhedron::~Polyhedron()
{
	if (pointList != NULL)
		delete pointList;
}

/**
 * Prints the polyhedra on the screen by calculating the intersection of three meshes 
 * each time.
 */
PointList *
Polyhedron::print()
{
    PointList * printing = new PointList();
    PointList::iterator iter;

    /* Going through the list of points and copy them into the new list. */
    for (iter = pointList->begin(); iter != pointList->end(); iter++) {
        printing->push_back(new Point(*(*iter)));
    }

    return printing;
}

