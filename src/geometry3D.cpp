/**
 * This file contains everything about 3D geometrical figures to be printed on the screen.
 *
 * @author  Roberto Fernandez Cueto
 * @date    12.10.2015
 *
 * $Id$
 */

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
Mesh::Mesh(Vector<3> vect, Point po)
{
	normal = Vector<3>(vect);
	point = Point(po);
}

Mesh::Mesh(const Mesh& mesh)
{
	normal = Vector<3>(mesh.normal);
	point = Point(mesh.point);
}

Mesh::Mesh(Point points[3])
{
    double trans[3][3] = {
        { 1.0,						1.0,						1.0 },
        { points[1].x - points[0].x, 	points[1].y - points[0].y, 	points[1].z - points[0].z },
        { points[2].x - points[0].x, 	points[2].y - points[0].y, 	points[2].z - points[0].z }
    };
	Matrix<3,3>	calc((const double **) trans);
    Matrix<2,2> adj;
	int idx;
    double mod = 1.0;

	point = Point(points[0]);
	normal = Vector<3>();

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
    double mesh[3][3] = {
        { m1.normal.getElement(0), m1.normal.getElement(1), m1.normal.getElement(2) },
		{ m2.normal.getElement(0), m2.normal.getElement(1), m2.normal.getElement(2) },
		{ m3.normal.getElement(0), m3.normal.getElement(1), m3.normal.getElement(2) }
    };
	Matrix<3, 3> meshMat((const double **) mesh), invert;
	Vector<3> cM1, cM2, cM3, cMeshes, result;

	/* Getting the vectors of the constant matrix. */
	cM1 = Vector<3>();
    cM1.setElement(0, m1.point.x);
    cM1.setElement(1, m1.point.y);
    cM1.setElement(2, m1.point.z);
	cM2 = Vector<3>();
    cM2.setElement(0, m2.point.x);
    cM2.setElement(1, m2.point.y);
    cM2.setElement(2, m2.point.z);
	cM3 = Vector<3>();
    cM3.setElement(0, m3.point.x);
    cM3.setElement(1, m3.point.y);
    cM3.setElement(2, m3.point.z);

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
    mode = GL_POLYGON;

    getPoints(&meshes);
}

Polyhedron::Polyhedron(Mesh ** mlist, unsigned int nmeshes)
{
    MeshList   meshes;
    unsigned int idx;

    /* Check the input. If list is NULL or the number of points is zero, return. */
    if (nmeshes == 0 || mlist == NULL)
        return;

    for (idx = 0; idx < nmeshes; idx++)
        meshes.push_back(mlist[idx]);

    mode = GL_POLYGON;
    getPoints(&meshes);
}

Polyhedron::Polyhedron(FaceList list)
{
    faces = FaceList(list);
    
    mode = GL_POLYGON;
    getOrigin();
}

Polyhedron::Polyhedron(Face ** list, unsigned int npoints)
{
    unsigned int idx;

    /* Inserting the list of vertices. */
    for (idx = 0; idx < npoints; idx++)
        faces.push_back(new Face(*list[idx]));
    mode = GL_POLYGON;
    getOrigin();
}
/**
 * Calculates the points where the meshes merge.
 * @param   MeshList    * meshList  The list of meshes whose points must be calculated.
 */
void
Polyhedron::getPoints(MeshList * meshList)
{
	MeshList::iterator	i1, i2, i3;
    PointList vert; /* Set a Vertices list. */
	PointList::iterator vit;
	Point	* temp;

    /* Going through the list of meshes and calculating the vertices of the polyhedron. */
	for (i1 = meshList->begin(); i1 != meshList->end(); i1++) {
        vert.clear();
		for (i2 = meshList->begin(); i2 != meshList->end(); i2++) {
			for (i3 = meshList->begin(); i3 != meshList->end(); i3++) {
                /* Getting the vertices of the face only if all the meshes are different. */
                if (i1 != i2 && i1 != i3 && i2 != i3) {
    				temp = Mesh::intersection(*(*i1), *(*i2), *(*i3));
	    			if (temp != NULL) {
		    			vert.push_back(temp);
                        vertices.push_back(temp);
                    }
                }
			}
		}
        faces.push_back(new Face(&vertices, &(*i1)->normal));
	}
    getOrigin();
}

/**
 * Calculates the center of the polyhedron.
 */
void
Polyhedron::getOrigin()
{
    PointList::iterator ita, itb;
    float dist, max_dist = 0.0;

    for (ita = vertices.begin(); ita != vertices.end(); ita++) {
        itb = ita;
        for (itb++; itb != vertices.end(); itb++) {
            dist = Point::distance(*(*ita), *(*itb));
            if (dist > max_dist) {
                org[0] = ((*ita)->x + (*itb)->x) / 2.0;
                org[1] = ((*ita)->y + (*itb)->y) / 2.0;
                org[2] = ((*ita)->z + (*itb)->z) / 2.0;
            }
        }
    }
}

/**
 * Prints the polyhedra on the screen by calculating the intersection of three meshes 
 * each time.
 */
FaceList *
Polyhedron::print()
{
    FaceList * printing = new FaceList();
    FaceList::iterator iter;

    /* Going through the list of points and copy them into the new list. */
    for (iter = faces.begin(); iter != faces.end(); iter++) {
        printing->push_back((*iter)->transform(org, angle));
    }

    return printing;
}

/**
 * Constructor of the prism.
 */
Prism::Prism(Point cBase1, Point cBase2, unsigned nsides, double radius)
{
    PointList bases[2], pointList, * current;
    Point   * point, centers[2] = {cBase1, cBase2};
    unsigned idx, base_idx;
    double angle = 2.0 * M_PI / nsides;
    Vector<3> dir((double *)Z_dir), ** walls;

    /* Allocate space for the vectors of the walls. */
    walls = (Vector<3> **) malloc(nsides * sizeof(Vector<3> *));
    if (walls == NULL)
        return;
    
    /* Setting the textures of the center. */
    for (idx = 0; idx < 2; idx++)
        centers[idx].s = centers[idx].t = 0.5;

    /* Calculating the points of the bases. */
    for (base_idx = 0; base_idx < 2; base_idx++) {
        for (idx = 0; idx < nsides; idx++) {
            point = new Point(centers[base_idx]);
            point->x += radius * cos(angle * idx);
            point->y += radius * sin(angle * idx);
            point->s = 0.5 * cos(angle);
            point->t = 0.5 * cos(angle);
            bases[base_idx].push_back(point);

            /* Calculate the normal to the face using the angle. */
            walls[idx] = new Vector<3>();
            walls[idx]->setElement(0, cos(angle));
            walls[idx]->setElement(1, sin(angle));
            walls[idx]->setElement(2, 0.0);
        }
    }

    /* Getting the faces of the prism as follows:
     *  -   First, the bases.
     *  -   Second, the faces of the walls.
     */

    /* First base. */
    faces.push_back(new Face(&bases[0], &dir));
    dir = dir * -1.0;
    
    /* Second base. */
    faces.push_back(new Face(&bases[0], &dir));
    

    bases[0].push_back(new Point(*bases[0].front()));
    bases[1].push_back(new Point(*bases[1].front()));

    /* Walls. */
    for (idx = 0; idx < nsides; idx++) {
        /* Clear the list of points. */
        pointList.clear();

        /* Get the points. */
        for (base_idx = 0; base_idx < 2; base_idx++) {
            current = &bases[base_idx];
            point = new Point(*current->front());

            current->pop_front();
            if (base_idx == 0) {
                pointList.push_back(point);
                pointList.push_back(new Point(*current->front()));
            } else {
                pointList.push_back(new Point(*current->front()));
                pointList.push_back(point);
            }
        }
        faces.push_back(new Face(&pointList, walls[idx]));
    }

    mode = GL_POLYGON;

    for (idx = 0; idx < nsides; idx++)
        delete walls[idx];
    free(walls);
}

