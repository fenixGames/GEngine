#include "matrix.h"
#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/**
 * Creates a vector with the indicated size.
 *
 * @param unsigned  nitems  The number of items of the vector.
 */
Vector::Vector(unsigned int nitems, ...)
{
    va_list list;
    unsigned int idx;

	elements = NULL;
	if (nitems == 0)
		return ;

	elements = new std::vector<double>(nitems);
	nelem = nitems;
	va_start(list, nitems);
	for (idx = 0; idx < nitems; idx++) {
		(*elements)[idx] = va_arg(list, double);
	}
	va_end(list);
}

Vector::Vector(const Vector& vect)
{
	nelem = vect.nelem;
	elements = new std::vector<double>(*vect.elements);
}
/**
 * Adds two vector and creates a new one.
 *
 * @param Vector    v1      The other vector to be added.
 *
 * @return The resulting vector.
 */
Vector
Vector::operator + (const Vector v1)
{
    Vector v2;
    unsigned int idx;

    /* Checking the size. */
    if (v1.nelem != nelem)
        return v2;
    
    v2.nelem = nelem;

    /* Adding the elements. */
    for (idx = 0; idx < nelem; idx++)
        (*v2.elements)[idx] = (*v1.elements)[idx] + (* elements)[idx];

    return v2;
}

/**
 * Substracts two vector and creates a new one.
 *
 * @param Vector    v1      The other vector to be substracted.
 *
 * @return The resulting vector.
 */
Vector
Vector::operator - (const Vector v1)
{
    Vector v2;
    unsigned int idx;

    /* Checking the size. */
    if (v1.nelem != nelem)
        return v2;
    
    v2.nelem = nelem;

    /* Adding the elements. */
    for (idx = 0; idx < nelem; idx++)
        (* v2.elements)[idx] = (* v1.elements)[idx] - (* elements)[idx];

    return v2;
}

/**
 * Calculates the scalar product of a vector.
 *
 * @param Vector    v1  The other vector to multiply.
 *
 * @return The scalar product.
 */
double
Vector::operator * (Vector v1)
{
    double value = 0.0;
    unsigned int idx;

    /* Checking the size. */
    if (nelem != v1.nelem)
        return 0.0;

    for (idx = 0; idx < nelem; idx++)
        value += (* elements)[idx] * (*v1.elements)[idx];

    return value;
}

/**
 * Multiplies an scalar to a vector and returns the result.
 *
 * @param double    value   The value to multiply.
 *
 * @return  The resulting vector.
 */
Vector
Vector::operator * (double value)
{
    Vector  v1;
    unsigned int idx;

	for (idx = 0; idx < nelem; idx++)
		(* v1.elements)[idx] = (* elements)[idx] * value;

    return v1;
}

/**
 * Assignates the same values of a vector to this one.
 *
 * @param	Vector	vect	The old vector.
 * @return 	The newly generated vector.
 */
Vector
Vector::operator = (Vector vect)
{
	nelem = vect.nelem;

	if (elements != NULL)
		delete elements;

	elements = new std::vector<double>(nelem);
    for (int idx = 0; idx < nelem; idx++)
        (*elements)[idx] = vect.elements->at(idx);

	return *this;
}

/**
 * Gets the element at the position indicated by the index.
 *
 * @param	unsigned	index	The index of the element to be retrieved.
 *
 * @return	The required element.
 */
double
Vector::getElement(unsigned int index)
{
	if (index < nelem)
		return (* elements)[index];
	else
		return 0.0;
}

/**
 * Sets the element at the position index to value.
 *
 * @param   unsigned    index   The index of the element to set.
 * @param   double      value   The new value of the element.
 */
void
Vector::setElement(unsigned int index, double value)
{
    if (index > nelem)
        return;

    (*elements)[index] = value;
}

/**
 * Destructor of the class.
 */
Vector::~Vector()
{
	delete elements;
}

#ifdef DEBUG
/**
 * Prints the vector on the screen.
 */
void
Vector::print() {
    printf("Vector %d\n[", nelem);

    for(int idx = 0; idx < nelem; idx++)
        printf(" %f,", elements->at(idx));
    printf("\b ]\n"); 
}
#endif

