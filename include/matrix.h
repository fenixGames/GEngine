/**
 * This file contains the information about vectors and matrices.
 *
 * @author  Roberto Fernandez Cueto
 * @date    23.10.2015
 *
 * $Id$
 */

#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <stdarg.h>
#include <array>

template < size_t N, size_t M> class Matrix;

/**
 * A class to define a vector.
 */
template < size_t N >
class Vector {
    protected:
		std::array<double, N> elements;
    public:
        Vector(double * values = NULL);
		Vector(const Vector& vect);
		~Vector();

        /* The scalar product of the vector. */
        double operator * (const Vector v1);

        /* The addition and substraction of two vectors. */
        Vector operator + (const Vector v1);
        Vector operator - (const Vector v1);

        /* Multiplying a constant to a Vector. */
        Vector operator * (double value);

		/* Asignates the same values to another vector. */
		Vector operator = (Vector vect);

        /* The same as getElement. */
        double operator [] (unsigned int i);

		/* Gets/Sets the element at position i. */
		double getElement(unsigned int i);
        void setElement(unsigned int i, double value);

        /* Calculates the modulus of the vector. */
        double mod();
#ifdef DEBUG
        void print();
#endif
};

/**
 * A class to define a matrix.
 */
template < size_t nrows, size_t ncolumns >
class Matrix {
    protected:
		std::array<double, nrows * ncolumns> matrix;
    public:
        Matrix(const double ** values = NULL);
		Matrix(const Matrix<nrows, ncolumns>& matrix);

        /* Gets the element in the (idx, idy) position. */
        double getElement (unsigned int row, unsigned int col) const;

        /* Adds or substracts two matrices. */
        Matrix  operator + (const Matrix m1);
        Matrix  operator - (const Matrix m1);

        /* Multiplies two matrices. */
        Matrix  operator * (const Matrix<ncolumns, nrows> m2);

        /* Escalates a matrix. */
        Matrix  operator * (const double value);

		/* Multiplies a matrix to a vector. */
		Vector<nrows> operator * (const Vector<ncolumns> vect);

		/* Copy asignment. */
		Matrix& operator= (const Matrix& mat);

        /* Makes the transponse of the matrix. */
        Matrix<ncolumns, nrows> transponse();

        /* Calculates the invert of the matrix. */
        Matrix invert();

        /* Calculates the determinant of the matrix. */
        double determinant() const;
        
		/* Sets an element of the matrix. */
		void setElement(unsigned int row, unsigned int col, double value);

		/* Returns the identity matrix. */
		static Matrix<nrows, ncolumns> identity();
        
		/* Retruns the adjoint matrix of the current one. */
		Matrix<nrows - 1, ncolumns - 1> getAdjoint(unsigned int row, unsigned int col) const;
#ifdef DEBUG
        /* Printing a matrix only makes sense on debug. */
        virtual void print() const;
#endif
};

#endif
