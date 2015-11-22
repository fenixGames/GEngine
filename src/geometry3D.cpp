#include "geometry.h"
#include <math.h>
#include <GL/glut.h>

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

	point = Point(points[0]);
	normal = Vector(3, 0.0, 0.0, 0.0);

	for (idx = 0; idx < 3; idx++) {
		adj = calc.getAdjoint(0, idx);
		normal.setElement(idx, adj.determinant());
	}
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
 * Constructor of the polyhedra.
 */
Polyhedron::Polyhedron(MeshList points)
{
    MeshList::iterator iter;

    meshList = new MeshList();

    for (iter = points.begin(); iter != points.end(); iter++)
        meshList->push_back(*iter);

    mode = GL_POLYGON;
}

Polyhedron::Polyhedron(Mesh ** mlist, unsigned int nmeshes)
{
    unsigned int idx;

	meshList = NULL;

    /* Check the input. If list is NULL or the number of points is zero, return. */
    if (nmeshes == 0 || mlist == NULL)
        return;

    meshList = new MeshList();
    for (idx = 0; idx < nmeshes; idx++)
        meshList->push_back(mlist[idx]);
    
    mode = GL_TRIANGLE_STRIP;
}

/**
 * Destructor of the polyhedron.
 */
Polyhedron::~Polyhedron()
{
	if (meshList != NULL)
		delete meshList;
}
#ifdef DEBUG
#include <stdio.h>
#endif
/**
 * Prints the polyhedra on the screen by calculating the intersection of three meshes 
 * each time.
 */
PointList *
Polyhedron::print()
{
    PointList * printing = new PointList();
	PointList vertices; /* Set a Vertices list. */
	PointList::iterator vit;
	MeshList::iterator	i1, i2, i3;
	Point	* temp;
	unsigned int nedges;

	if (meshList == NULL)
		return printing;

	/* Going through the list of meshes and calculating the vertices of the polyhedron. */
	for (i1 = meshList->begin(); i1 != meshList->end(); i1++) {
		i2 = i1;
		for (i2++; i2 != meshList->end(); i2++) {
			i3 = i2;
			for (i3++; i3 != meshList->end(); i3++) {
				temp = Mesh::intersection(*(*i1), *(*i2), *(*i3));
				printf( "temp = (%f, %f, %f)\n", temp->x, temp->y, temp->z);
				if (temp != NULL)
					vertices.push_back(temp);
			}
		}
	}

	/* Caculating the number of edges of the figure. */
	nedges = meshList->size() + vertices.size() - 2;
	while (!vertices.empty()) {
		temp = vertices.front();
		printing->push_back(new Point(temp->x, temp->y, temp->z));
		vertices.pop_front();
	}

 	nedges -= printing->size();
	vit = printing->begin();

	/* If there are not enough vertices on the printing list, add the first ones again. */
	while (nedges > 0) {
		temp = * vit;
		printing->push_back(new Point(temp->x, temp->y, temp->z));
		vit++;
		nedges--;
	}
    return printing;
}

