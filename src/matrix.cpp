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

    /* Allocating space for the matrix. */
    this->matrix = (double *) malloc(rows * cols * sizeof(double));
    if (this->matrix == NULL) { // Failed to allocate the matrix.
        this->nrows = 0;
        this->ncolumns = 0;
    } else {
        this->nrows = rows;
        this->ncolumns = cols;

        /* Going through the list of arguments and storing the values. */
        va_start(list, cols);
        for (idx = 0; idx < this->nrows; idx++) {
            for (idy = 0; idy < this->ncolumns; idy++) {
                item = va_arg(list, double);
                this->setElement(idx, idy, item);
            }
        }
        va_end(list);
    }
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
    if (row >= this->nrows || col >= this->ncolumns)
        return 0.0;

    return *(this->matrix + (row * this->ncolumns) + col);
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
    if (m1.nrows != this->nrows || this->ncolumns != m1.ncolumns)
        return m3;

    /* Allocating memory for the matrix. */
    m3.matrix = (double *) malloc(this->nrows * this->ncolumns * sizeof(double));
    if (m3.matrix != NULL) {
        m3.nrows = this->nrows;
        m3.ncolumns = this->ncolumns;

        /* Adding the matrices and storing the result in the new one. */
        for (idx = 0; idx < this->nrows; idx++) {
            for (idy = 0; idy < this->ncolumns; idy++)
                m3.setElement(idx, idy, m1.getElement(idx, idy) + this->getElement(idx, idy));
        }
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
    if (m1.nrows != this->nrows || this->ncolumns != m1.ncolumns)
        return m3;

    /* Allocating memory for the matrix. */
    m3.matrix = (double *) malloc(this->nrows * this->ncolumns * sizeof(double));
    if (m3.matrix != NULL) {
        m3.nrows = this->nrows;
        m3.ncolumns = this->ncolumns;

        /* Adding the matrices and storing the result in the new one. */
        for (idx = 0; idx < this->nrows; idx++) {
            for (idy = 0; idy < this->ncolumns; idy++)
                m3.setElement(idx, idy, m1.getElement(idx, idy) - this->getElement(idx, idy));
        }
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
    Matrix m3;
    unsigned int idx, idy, idz;
    double value;

    /* Checking the dimensions. */
    if (this->ncolumns != m2.nrows)
        return m3;


    m3.nrows = this->nrows;
    m3.ncolumns = m2.ncolumns;

    /* Allocating memory for the matrix. */
    m3.matrix = (double *) malloc(m3.nrows * m3.ncolumns * sizeof(double));
    if (m3.matrix == NULL) {
        return m3;
    }

    /* Creating the new matrix. */
    for (idx = 0; idx < m3.nrows; idx++) {
        for (idy = 0; idy < m3.ncolumns; idy++) {
            /* m3(idx, idy) = sum(k, 0, M, this->matrix(idx, k) * this->matrix(k, idy)) */
            value = 0.0;
            for (idz = 0; idz < this->ncolumns; idz++)
                value += this->getElement(idx, idz) * m2.getElement(idz, idy);
            m3.setElement(idx, idy, value);
        }
    }

    return m3;
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

    /* Creating the new matrix. */
    result.matrix = (double *)malloc(this->nrows * this->ncolumns * sizeof(double));
    if (result.matrix == NULL) {
        return result;
    }
    result.nrows        = this->nrows;
    result.ncolumns     = this->ncolumns;

    /* Calculating the elements of the matrix. */
    for (idx = 0; idx < this->nrows; idx++)
        for (idy = 0; idy < this->ncolumns; idy++)
            result.setElement(idx, idy, value * this->getElement(idx, idy));

    return result;
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

    trans.nrows     = this->ncolumns;
    trans.ncolumns  = this->nrows;

    /* Creating the matrix. */
    trans.matrix = (double *) malloc(trans.nrows * trans.ncolumns * sizeof(double));
    if (trans.matrix == NULL)
        return trans;

    /* Setting the elements. */
    for (idx = 0; idx < trans.nrows; idx++)
        for (idy = 0; idy < trans.ncolumns; idy++)
            trans.setElement(idx, idy, this->getElement(idy, idx));
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
    if (this->nrows != this->ncolumns)
        return adj;

    adj.matrix = (double *) malloc((this->nrows - 1) * (this->ncolumns - 1) * sizeof(double));
    if (adj.matrix == NULL)
        return adj;

    adj.nrows       = this->nrows - 1;
    adj.ncolumns    = this->ncolumns - 1;
    /* Getting the adjoint matrix. */
    for (idx = 0, ida = 0; idx < this->nrows; idx++) {
        for (idy = 0; idy < this->ncolumns; idy++) {
            if (idx != row && idy != col) {
                adj.matrix[ida] = this->getElement(idx, idy);
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
    if (this->nrows != this->ncolumns)
        return 0.0;

    /* If the matrix is 1 x 1, return the value. */
    if (this->nrows == 1)
        return this->matrix[0];

    /* Getting the determinant through the adjoint of the matrix. */
    for (fixed = 0; fixed < this->nrows; fixed++) {
        adj = this->getAdjoint(0, fixed);

        /* Calculating the determinant. */
        value += mod * adj.determinant() * this->matrix[fixed];
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
    Matrix invert, cof;
    double det = this->determinant(), mod = 1.0;
    unsigned int idx, idy;

    /* If the determinant is zero, it could mean that the matrix is not square. */
    if (det == 0)
        return invert;

    /* Calculating the cofactor matrix. */
    cof.matrix = (double *) malloc(this->nrows * this->ncolumns * sizeof(double));
    if (cof.matrix == NULL)
        return invert;

    cof.nrows = this->nrows;
    cof.ncolumns = this->ncolumns;
    for (idx = 0; idx < this->nrows; idx++)
        for (idy = 0; idy < this->ncolumns; idy++) {
            cof.setElement(idx, idy, this->getAdjoint(idx, idy).determinant() * mod);
            mod *= -1.0;
        }

    /* Calculating the inverse. */
    det = 1 / det;

    invert = cof.transponse() * det;

    return invert;
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
    if (row >= this->nrows || col >= this->ncolumns)
        return;

    *(this->matrix + (row * this->ncolumns) + col) = value;
}

/**
 * Creates a vector with the indicated size.
 *
 * @param unsigned  nitems  The number of items of the vector.
 */
Vector::Vector(unsigned int nitems, ...)
{
    va_list list;
    unsigned int idx;

    this->elements = (double *) malloc(nitems * sizeof(double));
    if (this->elements == NULL)
        this->nelem = 0;
    else {
        this->nelem = nitems;
        va_start(list, nitems);
        for (idx = 0; idx < nitems; idx++) {
            this->elements[idx] = va_arg(list, double);
        }
        va_end(list);
    }
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
    if (v1.nelem != this->nelem)
        return v2;
    
    v2.elements = (double *) malloc(this->nelem * sizeof(double));
    if (v2.elements == NULL)
        return v2;

    v2.nelem = this->nelem;

    /* Adding the elements. */
    for (idx = 0; idx < this->nelem; idx++)
        v2.elements[idx] = v1.elements[idx] + this->elements[idx];

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
    if (v1.nelem != this->nelem)
        return v2;
    
    v2.elements = (double *) malloc(this->nelem * sizeof(double));
    if (v2.elements == NULL)
        return v2;

    v2.nelem = this->nelem;

    /* Adding the elements. */
    for (idx = 0; idx < this->nelem; idx++)
        v2.elements[idx] = v1.elements[idx] - this->elements[idx];

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
    if (this->nelem != v1.nelem)
        return 0.0;

    for (idx = 0; idx < this->nelem; idx++)
        value += this->elements[idx] * v1.elements[idx];

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

    v1.elements = (double *) malloc(this->nelem * sizeof(double));
    if (v1.elements != NULL) {
        for (idx = 0; idx < this->nelem; idx++)
            v1.elements[idx] = this->elements[idx] * value;
    }

    return v1;
}

#ifdef DEBUG
void
Matrix::print()
{
    unsigned int idx, idy;

    printf("%u x %x matrix:\n", this->nrows, this->ncolumns);
    for (idx = 0; idx < this->nrows; idx++) {
        for (idy = 0; idy < this->ncolumns; idy++) {
            printf("%-4.4f ", *(this->matrix + (idx * this->ncolumns + idy)));
        }
        printf("\n");
    }
}
#endif

