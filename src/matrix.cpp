#include "matrix.h"
#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/**
 * Creates the matrix with the fixed rows and columns and taking the rest of arguments from the va_list.
 *
 * @param unsigned  rows    The number of rows for the matrix.
 * @param unsigned  cols    The number of columns for the matrix.
 */
Matrix::Matrix(unsigned int rows, unsigned int cols, ...)
{
    va_list list;
    unsigned int idx, idy;
    double  item;

    nrows = rows;
    ncolumns = cols;

	matrix = new std::vector<double>(rows * cols, 0.0);

    /* Going through the list of arguments and storing the values. */
    va_start(list, cols);
    for (idx = 0; idx < nrows; idx++) {
	    for (idy = 0; idy < ncolumns; idy++) {
			item = va_arg(list, double);
			setElement(idx, idy, item);
		}
	}
	va_end(list);
}

/**
 * Asignates all the elements of a matrix to the new one.
 *
 * @param	Matrix	mat		The matrix whose values are reasignated.
 *
 * @return	The new matrix.
 */
Matrix::Matrix(const Matrix& mat)
{
	unsigned int idx, idy;

	nrows = mat.nrows;
	ncolumns = mat.ncolumns;

	matrix = new std::vector<double>(nrows * ncolumns);

	/* Setting the new values. */
	for (idx = 0; idx < nrows; idx++)
		for (idy = 0; idy < ncolumns; idy++)
			setElement(idx, idy, mat.getElement(idx, idy));

}

Matrix::~Matrix()
{
	delete matrix;
}

/**
 * Retrieves the element in the (row, col) position.
 *
 * @param   unsigned    row     The row where the element is stored.
 * @param   unsigned    col     The column where the element is stored.
 *
 * @return  The element in that position.
 */
double
Matrix::getElement(unsigned int row, unsigned int col) const
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
Matrix  
Matrix::operator + (const Matrix m1)
{
    Matrix m3;
    unsigned int idx, idy;

    /* Check sizes. */
    if (m1.nrows != nrows || ncolumns != m1.ncolumns)
        return m3;

	m3.nrows = nrows;
	m3.ncolumns = ncolumns;

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
Matrix  
Matrix::operator - (const Matrix m1)
{
    Matrix m3;
    unsigned int idx, idy;

    /* Check sizes. */
    if (m1.nrows != nrows || ncolumns != m1.ncolumns)
        return m3;

	m3.nrows = nrows;
	m3.ncolumns = ncolumns;

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
Matrix 
Matrix::operator * (const Matrix m2)
{
    Matrix * m3 = new Matrix();
    unsigned int idx, idy, idz;
    double value;

    /* Checking the dimensions. */
    if (ncolumns != m2.nrows)
        return * m3;

    m3->nrows = nrows;
    m3->ncolumns = m2.ncolumns;
	m3->matrix = new std::vector<double>(m3->nrows * m3->ncolumns);

    /* Creating the new matrix. */
    for (idx = 0; idx < m3->nrows; idx++) {
        for (idy = 0; idy < m3->ncolumns; idy++) {
            /* m3(idx, idy) = sum(k, 0, M, matrix(idx, k) * matrix(k, idy)) */
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
Matrix 
Matrix::operator * (double value)
{
    Matrix result;
    unsigned int idx, idy;

    result.nrows        = nrows;
    result.ncolumns     = ncolumns;
	result.matrix		= new std::vector<double>(result.nrows * result.ncolumns);

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
Vector
Matrix::operator * (const Vector vect)
{
	Vector	result;
	unsigned int idx, idy;

	/* Checking the sizes. */
	if (ncolumns != vect.nelem)
		return result;

	result.nelem = nrows;
	result.elements = new std::vector<double>(nrows);
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
Matrix&
Matrix::operator = (const Matrix& mat)
{
	unsigned int idx, idy;

	/* Empty the current matrix. */
	matrix->clear();

	/* Resize it. */
	matrix->resize(mat.nrows * mat.ncolumns);
	nrows = mat.nrows;
	ncolumns = mat.ncolumns;

	/* Set the new elements. */
	for (idx = 0; idx < nrows; idx++)
		for (idy = 0; idy < ncolumns; idy++)
			setElement(idx, idy, mat.getElement(idx, idy));

	return * this;
}

/**
 * Calculates the transponse of a Matrix. That would be to switch rows and columns.
 *
 * @return  The transponse of the matrix.
 */
Matrix
Matrix::transponse()
{
    Matrix trans;
    unsigned int idx, idy;

    trans.nrows     = ncolumns;
    trans.ncolumns  = nrows;
	trans.matrix	 = new std::vector<double>(trans.nrows * trans.ncolumns);

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
Matrix
Matrix::getAdjoint(unsigned int row, unsigned int col) const
{
    unsigned int idx, idy, ida;
    Matrix adj;

    /* Checking the size. */
    if (nrows != ncolumns)
        return adj;

    adj.nrows       = nrows - 1;
    adj.ncolumns    = ncolumns - 1;

	adj.matrix = new std::vector<double>(adj.nrows * adj.ncolumns);
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
double
Matrix::determinant() const
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
Matrix
Matrix::invert()
{
    Matrix invert, cof, trans;
    double det = determinant(), mod = 1.0;
    unsigned int idx, idy;

    /* If the determinant is zero, it could mean that the matrix is not square. */
    if (det == 0)
        return * (new Matrix());

    cof.nrows = nrows;
    cof.ncolumns = ncolumns;
	cof.matrix = new std::vector<double>(cof.nrows * cof.ncolumns);
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
void
Matrix::setElement(unsigned int row, unsigned int col, double value)
{
    if (row >= nrows || col >= ncolumns)
        return;

    (*matrix)[(row * ncolumns) + col] = value;
}
#ifdef DEBUG
void
Matrix::print() const
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
Matrix
Matrix::identity(int size)
{
	Matrix * mat = new Matrix();
	int idx, idy;

	mat->nrows = size;
	mat->ncolumns = size;
	mat->matrix = new std::vector<double>(size * size);

	/* Setting the values. */
	for (idx = 0; idx < size; idx++)
		for (idy = 0; idy < size; idy++)
			mat->setElement(idx, idy, (idx == idy ? 1 : 0));

	return * mat;
}

