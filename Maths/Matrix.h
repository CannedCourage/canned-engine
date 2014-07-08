#ifndef _MATRIX_H_
#define _MATRIX_H_

//Matrix(m)x(n), where m = number of rows, n = number of columns
//Matrix multiplication
//Matrix A is m x n, matrix B is n x p, result is an m x p matrix

#include <iostream>
#include <vector>

class Matrix
{
private:
protected:

	unsigned int rows, columns;
	std::vector< std::vector<double> > elements;
public:

	Matrix( const unsigned int r = 2, const unsigned int c = 2 );
	Matrix( const Matrix& mat );
	Matrix& operator=( const Matrix& rhs );

	double& operator()( const unsigned int row, const unsigned int column );
	const double operator()( const unsigned int row, const unsigned int column ) const;

	friend const Matrix operator*( const Matrix& lhs, const Matrix& rhs );
	friend const Matrix operator*( const double lhs, const Matrix& rhs );
	friend const Matrix operator*( const Matrix& lhs, const double rhs );

	Matrix& operator*=( const Matrix& rhs );
	Matrix& operator*=( const double rhs );

	Matrix& operator+=( const Matrix& rhs );
	Matrix& operator-=( const Matrix& rhs );

	friend const Matrix operator+( const Matrix& lhs, const Matrix& rhs );
	friend const Matrix operator-( const Matrix& lhs, const Matrix& rhs );

	void MakeIdentity( void );

	void Invert( void );
	const Matrix Inverse( void );

	void Transpose( void );
	const Matrix Transposed( void );

	friend double MatrixDeterminant( const Matrix& mat );

	double Determinant( void );

	double Trace( void );

	const bool IsSquare( void ) const;

	friend std::ostream& operator<<(std::ostream& os, const Matrix& mat);
};

#endif //_MATRIX_H_