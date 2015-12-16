/**
 * This file contains all the vectorial functions.
 *
 * @author  Roberto Fernandez Cueto
 * @date    26.11.2015
 *
 * $Id$
 */

#include "matrix.h"
#include <stdlib.h>
#include <math.h>

#ifdef DEBUG
#include <stdio.h>
#endif

using namespace std;

/**
 * Creates a vector with the indicated size.
 *
 * @param   double  * values    The array of values for the vector or NULL if none.
 */
template <size_t N>
Vector<N>::Vector(double * values)
{
    for (int idx = 0; idx < N && values; idx++)
        elements[idx] = values[idx];
}

template <size_t N>
Vector<N>::Vector(const Vector& vect)
{
	elements = new std::array<double, N>(*vect.elements);
}
/**
 * Adds two vector and creates a new one.
 *
 * @param Vector    v1      The other vector to be added.
 *
 * @return The resulting vector.
 */
template <size_t N>
Vector<N>
Vector<N>::operator + (const Vector<N> v1)
{
    Vector v2;
    unsigned int idx;

    v2.elements = std::array<double, N>(elements);

    for (idx = 0; idx < N; idx ++)
        v2.elements[idx] += v1.elements[idx];

    return v2;
}

/**
 * Substracts two vector and creates a new one.
 *
 * @param Vector    v1      The other vector to be substracted.
 *
 * @return The resulting vector.
 */
template <size_t N>
Vector<N>
Vector<N>::operator - (const Vector<N> v1)
{
    Vector v2;
    unsigned int idx;
    
    v2.elements = std::array<double, N>(elements);

    for (idx = 0; idx < N; idx ++)
        v2.elements[idx] -= v1.elements[idx];
    return v2;
}

/**
 * Calculates the scalar product of a vector.
 *
 * @param Vector    v1  The other vector to multiply.
 *
 * @return The scalar product.
 */
template <size_t N>
double
Vector<N>::operator * (Vector<N> v1)
{
    double value = 0.0;
    unsigned int idx;

    for (idx = 0; idx < N; idx++)
        value += elements[idx] * v1.elements[idx];

    return value;
}

/**
 * Multiplies an scalar to a vector and returns the result.
 *
 * @param double    value   The value to multiply.
 *
 * @return  The resulting vector.
 */
template <size_t N>
Vector<N>
Vector<N>::operator * (double value)
{
    Vector  v1 = Vector(*this);
    unsigned int idx;

	for (idx = 0; idx < N; idx++)
		v1.elements[idx] = elements[idx] * value;

    return v1;
}

/**
 * Assignates the same values of a vector to this one.
 *
 * @param	Vector	vect	The old vector.
 * @return 	The newly generated vector.
 */
template <size_t N>
Vector<N>
Vector<N>::operator = (Vector<N> vect)
{
	if (elements != NULL)
		delete elements;

	elements = new std::array<double, N>(elements);

	return *this;
}

/**
 * An alias for getElement.
 */
template <size_t N>
double
Vector<N>::operator [] (unsigned int index) {
    return getElement(index);
}

/**
 * Gets the element at the position indicated by the index.
 *
 * @param	unsigned	index	The index of the element to be retrieved.
 *
 * @return	The required element.
 */
template <size_t N>
double
Vector<N>::getElement(unsigned int index)
{
	if (index < N)
		return elements[index];
	else
		return 0.0;
}

/**
 * Sets the element at the position index to value.
 *
 * @param   unsigned    index   The index of the element to set.
 * @param   double      value   The new value of the element.
 */
template <size_t N>
void
Vector<N>::setElement(unsigned int index, double value)
{
    if (index > N)
        return;

    elements[index] = value;
}

/**
 * Destructor of the class.
 */
template <size_t N>
Vector<N>::~Vector()
{
	delete elements;
}

/**
 * Calculates the modulus of the vector and returns it.
 * @return  The modulus of the vector.
 */
template <size_t N>
double
Vector<N>::mod()
{
    return sqrt(getElement(0) * getElement(0) + getElement(1) * getElement(1) + 
            getElement(2) * getElement(2));
}

#ifdef DEBUG
/**
 * Prints the vector on the screen.
 */
template <size_t N>
void
Vector<N>::print() {
    printf("Vector %d\n[", N);

    for(unsigned int idx = 0; idx < N; idx++)
        printf(" %f,", elements->at(idx));
    printf("\b ]\n"); 
}
#endif

