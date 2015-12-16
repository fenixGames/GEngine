/**
 * This file contains the description of all the matricial functions.
 *
 * @author  Roberto Fernandez Cueto
 * @date    26.11.2015
 *
 * $Id$
 */

#include "matrix.h"
#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/**
 * Creates the matrix with the fixed rows and columns and taking the rest of arguments from the va_list.
 */
template <size_t nrows, size_t ncolumns>
Matrix<nrows, ncolumns>::Matrix(const double ** values)
{
    for (int idx = 0; idx < nrows && values; idx++)
        for (int idy = 0; idy < ncolumns && values[idx]; idx++)
            matrix[idx * ncolumns + idx] = values[idx][idy];
}

/**
 * Asignates all the elements of a matrix to the new one.
 *
 * @param	Matrix	mat		The matrix whose values are reasignated.
 *
 * @return	The new matrix.
 */
template <size_t nrows, size_t ncolumns>
Matrix<nrows, ncolumns>::Matrix(const Matrix<nrows, ncolumns>& mat)
{
	matrix = std::array<double, nrows * ncolumns>(mat.elements);
}

/**
 * Retrieves the element in the (row, col) position.
 *
 * @param   unsigned    row     The row where the element is stored.
 * @param   unsigned    col     The column where the element is stored.
 *
 * @return  The element in that position.
 */
template <size_t nrows, size_t ncolumns>
double
Matrix<nrows, ncolumns>::getElement(unsigned int row, unsigned int col) const
{
    if (row >= nrows || col >= ncolumns)
        return 0.0;

    return matrix->at((row * ncolumns) + col);
}

/**
 * Adds two matrices into a new one.
 *
 * @param   Matrix  m1  The first matrix.
 * @param   Matrix  m2  The second matrix.
 *
 * @return  The new Matrix if the size of the matrices are the right one or an empty matrix otherwise.
 */
template <size_t nrows, size_t ncolumns>
Matrix<nrows, ncolumns>
Matrix<nrows, ncolumns>::operator + (const Matrix<nrows, ncolumns> m1)
{
    Matrix<nrows, ncolumns> m3;
    unsigned int idx, idy;

	/* Adding the matrices and storing the result in the new one. */
	for (idx = 0; idx < nrows; idx++) {
		for (idy = 0; idy < ncolumns; idy++)
			m3.setElement(idx, idy, m1.getElement(idx, idy) + getElement(idx, idy));
	}

    return m3;
}
/**
 * Substracts two matrices into a new one.
 *
 * @param   Matrix  m1  The first matrix.
 * @param   Matrix  m2  The second matrix.
 *
 * @return  The new Matrix if the size of the matrices are the right one or an empty matrix otherwise.
 */
template <size_t nrows, size_t ncolumns>
Matrix<nrows, ncolumns>
Matrix<nrows, ncolumns>::operator - (const Matrix<nrows, ncolumns> m1)
{
    Matrix<nrows, ncolumns> m3;
    unsigned int idx, idy;

	/* Adding the matrices and storing the result in the new one. */
	for (idx = 0; idx < nrows; idx++) {
		for (idy = 0; idy < ncolumns; idy++)
			m3.setElement(idx, idy, m1.getElement(idx, idy) - getElement(idx, idy));
	}

    return m3;
}

/**
 * Multiplies the two matrices returning the resulting matrix.
 * The sizes of the matrices must (and will) be the following:
 *      ( N x O ) = ( N x M ) * ( M x O )
 *
 * @param   Matrix  m2  The matrix to be multiplied which size is M x O.
 * @return  The result of the product or NULL if they cannot be multiplied.
 */
template <size_t nrows, size_t ncolumns>
Matrix<nrows, ncolumns>
Matrix<nrows, ncolumns>::operator * (const Matrix<ncolumns, nrows> m2)
{
    Matrix * m3 = new Matrix();
    unsigned int idx, idy, idz;
    double value;

	m3->matrix = std::array<double, nrows * ncolumns>();

    /* Creating the new matrix. */
    for (idx = 0; idx < nrows; idx++) {
        for (idy = 0; idy < ncolumns; idy++) {
            value = 0.0;
            for (idz = 0; idz < ncolumns; idz++)
                value += getElement(idx, idz) * m2.getElement(idz, idy);
            m3->setElement(idx, idy, value);
        }
    }

    return * m3;
}

/**
 * Multiplies a value to all the elements of the matrix.
 *
 * @param   double  value   The value to be multiplied to each element of the matrix.
 * @return  The resulting matrix.
 */
template <size_t nrows, size_t ncolumns>
Matrix<nrows, ncolumns>
Matrix<nrows, ncolumns>::operator * (double value)
{
    Matrix result;
    unsigned int idx, idy;

	result.matrix		= std::array<double, nrows * ncolumns>();

    /* Calculating the elements of the matrix. */
    for (idx = 0; idx < nrows; idx++)
        for (idy = 0; idy < ncolumns; idy++)
            result.setElement(idx, idy, value * getElement(idx, idy));

    return result;
}

/**
 * Multiplies a matrix to a vector getting another vector. The size of the vector must be the
 * same as the number of columns in the matrix, resulting a new vector with a size equal to
 * the number of rows of the matrix.
 *
 * @param Vector	vect	The vector to be multiplied to the matrix.
 *
 * @return	The new vector.
 */
template <size_t nrows, size_t ncolumns>
Vector<nrows>
Matrix<nrows, ncolumns>::operator * (const Vector<ncolumns> vect)
{
	Vector<ncolumns>	result;
	unsigned int idx, idy;

	result.elements = std::array<double, nrows>();
	/* Multipling ... */
	for (idx = 0; idx < nrows; idx++) {
		(* result.elements)[idx] = 0;
		for (idy = 0; idy < ncolumns; idy++) {
			(*result.elements)[idx] += getElement(idx, idy) * (* vect.elements)[idy];
		}
	}

	return result;
}

/**
 * Asignates the resources of the matrix to a new one.
 *
 * @param	Matrix	mat	The matrix to copy.
 * @return	The new matrix.
 */
template <size_t nrows, size_t ncolumns>
Matrix<nrows, ncolumns>&
Matrix<nrows, ncolumns>::operator = (const Matrix& mat)
{
    matrix = std::array<double, nrows * ncolumns>(mat.matrix);

	return * this;
}

/**
 * Calculates the transponse of a Matrix. That would be to switch rows and columns.
 *
 * @return  The transponse of the matrix.
 */
template <size_t nrows, size_t ncolumns>
Matrix<ncolumns, nrows>
Matrix<nrows, ncolumns>::transponse()
{
    Matrix trans;
    unsigned int idx, idy;

	trans.matrix	 = std::array<double, nrows * ncolumns>();

    /* Setting the elements. */
    for (idx = 0; idx < trans.nrows; idx++)
        for (idy = 0; idy < trans.ncolumns; idy++)
            trans.setElement(idx, idy, getElement(idy, idx));
    return trans;
}

/**
 * Gets the adjoint matrix of the current one at the (row, col) position.
 *
 * @param   unsigned    row     The row where the adjoint will be calculated.
 * @param   unsigned    col     The column where the adjoint will be calculated.
 *
 * @return The adjoint matrix.
 */
template <size_t nrows, size_t ncolumns>
Matrix<nrows - 1, ncolumns - 1>
Matrix<nrows, ncolumns>::getAdjoint(unsigned int row, unsigned int col) const
{
    unsigned int idx, idy, ida;
    Matrix adj;

	adj.matrix = std::array<double, (nrows - 1) * (ncolumns - 1)>();
    /* Getting the adjoint matrix. */
    for (idx = 0, ida = 0; idx < nrows; idx++) {
        for (idy = 0; idy < ncolumns; idy++) {
            if (idx != row && idy != col) {
                (*adj.matrix)[ida] = getElement(idx, idy);
                ida++;
            }
        }
    }

    return adj;
}

/**
 * Calculates the determinant of a Matrix.
 *
 * @return The determinant.
 */
template <size_t nrows, size_t ncolumns>
double
Matrix<nrows, ncolumns>::determinant() const
{
    double value = 0.0, mod = 1.0;
    unsigned int  fixed;
    Matrix adj;

    /* The determinant can only be calculated for square matrices. */
    if (nrows != ncolumns)
        return 0.0;

    /* If the matrix is 1 x 1, return the value. */
    if (nrows == 1)
        return  matrix->at(0);

    /* Getting the determinant through the adjoint of the matrix. */
    for (fixed = 0; fixed < nrows; fixed++) {
        adj = getAdjoint(0, fixed);

        /* Calculating the determinant. */
        value += mod * adj.determinant() *  matrix->at(fixed);
        mod *= -1.0;
    }

    return value;
}

/**
 * Calculates the invert of the matrix.
 *
 * @return The invert of the matrix.
 */
template <size_t nrows, size_t ncolumns>
Matrix<nrows, ncolumns>
Matrix<nrows, ncolumns>::invert()
{
    Matrix invert, cof, trans;
    double det = determinant(), mod = 1.0;
    unsigned int idx, idy;

    /* If the determinant is zero, it could mean that the matrix is not square. */
    if (det == 0)
        return * (new Matrix());

	cof.matrix = std::array<double, nrows * ncolumns>();
    for (idx = 0; idx < nrows; idx++)
        for (idy = 0; idy < ncolumns; idy++) {
            cof.setElement(idx, idy, getAdjoint(idx, idy).determinant() * mod);
            mod *= -1.0;
        }

    /* Calculating the inverse. */
    det = 1 / det;

	trans = cof.transponse();
    invert = cof.transponse() * det;

    return * (new Matrix(invert));
}

/**
 * Sets the value in the position indicated.
 *
 * @param unsigned  row     The row where the element must be stored.
 * @param unsigned  col     The column where the element must be stored.
 * @param double    value   The value to store.
 */
template <size_t nrows, size_t ncolumns>
void
Matrix<nrows, ncolumns>::setElement(unsigned int row, unsigned int col, double value)
{
    if (row >= nrows || col >= ncolumns)
        return;

    (*matrix)[(row * ncolumns) + col] = value;
}
#ifdef DEBUG
template <size_t nrows, size_t ncolumns>
void
Matrix<nrows, ncolumns>::print() const
{
    unsigned int idx, idy;

    printf("%u x %x matrix:\n", nrows, ncolumns);
    for (idx = 0; idx < nrows; idx++) {
        for (idy = 0; idy < ncolumns; idy++) {
            printf("%-4.4f ", getElement(idx, idy));
        }
        printf("\n");
    }
}
#endif

/**
 * Returns the identity matrix of size 'size' x 'size'.
 *
 * @param	int		size	The size of the matrix.
 *
 * @return 	The identity matrix.
 */
template <size_t nrows, size_t ncolumns>
Matrix<nrows, ncolumns>
Matrix<nrows, ncolumns>::identity()
{
	Matrix * mat = new Matrix();
	int idx, idy;

	mat->matrix = std::array<double, nrows * ncolumns>();

	/* Setting the values. */
	for (idx = 0; idx < nrows; idx++)
		for (idy = 0; idy < ncolumns; idy++)
			mat->setElement(idx, idy, (idx == idy ? 1 : 0));

	return * mat;
}

