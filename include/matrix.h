/**
 * This file contains the information about vectors and matrices.
 *
 * @author  Roberto Fernandez Cueto
 * @date    23.10.2015
 */

#ifndef _MATRIX_H
#define _MATRIX_H
#include <stdarg.h>
#include <vector>

class Matrix;

/**
 * A class to define a vector.
 */
class Vector {
		friend class Matrix;
    protected:
        unsigned int nelem;
		std::vector<double> * elements;
    public:
        Vector(unsigned int nitems = 0, ...);
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

		/* Getting the element at position i. */
		double getElement(unsigned int i);
};

/**
 * A class to define a matrix.
 */
class Matrix {
    protected:
        unsigned int nrows, ncolumns;
		std::vector<double> * matrix;

        Matrix getAdjoint(unsigned int row, unsigned int col) const;
    public:
        Matrix(unsigned int rows = 0, unsigned int cols = 0, ...);
		virtual ~Matrix();

        /* Gets the element in the (idx, idy) position. */
        double getElement (unsigned int row, unsigned int col) const;

        /* Adds or substracts two matrices. */
        Matrix  operator + (const Matrix m1);
        Matrix  operator - (const Matrix m1);

        /* Multiplies two matrices. */
        Matrix  operator * (const Matrix m2);

        /* Escalates a matrix. */
        Matrix  operator * (const double value);

		/* Multiplies a matrix to a vector. */
		Vector operator * (const Vector vect);

		/* Copies the values of the matrix to the new matrix. */
		Matrix operator = (const Matrix mat);

        /* Makes the transponse of the matrix. */
        Matrix transponse();

        /* Calculates the invert of the matrix. */
        Matrix invert();

        /* Calculates the determinant of the matrix. */
        double determinant() const;
        
		/* Sets an element of the matrix. */
		void setElement(unsigned int row, unsigned int col, double value);

		/* Returns the identity matrix. */
		static Matrix * identity(int size);
#ifdef DEBUG
        /* Printing a matrix only makes sense on debug. */
        virtual void print() const;
#endif
};

#endif
