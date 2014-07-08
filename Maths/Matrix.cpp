#include "Maths/Matrix.h"
#include <stdexcept>
using std::invalid_argument;

Matrix::Matrix( const unsigned int r, const unsigned int c ) : rows( r ), columns( c )
{
    if( ( rows > 0 ) && ( columns > 0 ) )
    {
        elements.resize( rows );
        
        for( unsigned int i = 0; i < rows; i++ )
        {
            elements[i].resize( columns );
        }
    }
    else
    {
        throw invalid_argument( "Matrix cannot have rows/columns less that one" );
    }
}

Matrix::Matrix( const Matrix& mat ) : rows( mat.rows ), columns( mat.columns )
{
    if( ( rows > 0 ) && ( columns > 0 ) )
    {
        elements.resize( rows );
        
        for( unsigned int i = 0; i < rows; i++ )
        {
            elements[i].resize( columns );
        }
        
        for( unsigned int i = 0; i < rows; i++ )
        {
            for( unsigned int j = 0; j < columns; j++ )
            {
                elements[i][j] = mat( i, j );
            }
        }
    }
    else
    {
        throw invalid_argument( "Matrix cannot have rows/columns less that one" );
    }
}

void Matrix::MakeIdentity( void )
{
	//This only really makes sense for n x n matrices
	for( unsigned int i = 0; i < rows; i++ )
	{
		elements[i][i] = 1.0f;
	}
}

void Matrix::Invert( void )
{
	( *this ) = Inverse();
}

const Matrix Matrix::Inverse( void )
{
    if( !( IsSquare() ) ){ throw std::exception( "Can only calculate inverse of a square matrix" ); }
        
        if( rows == 1 )
        {
            //If matrix is 1x1, return the reciprocal of the value;
			Matrix temp( 1, 1 );

			temp( 0, 0 ) = ( 1 / elements[0][0] );

            return temp;
        }
        
        Matrix matrixOfCoefficients( rows, columns );
        
        int sign = 1;
        
        for( unsigned int r = 0; r < rows; r++ )
        {
            for( unsigned int c = 0; c < columns; c++ )
            {
                //r/c == current row/column being ignored
                
                Matrix _minor( ( rows - 1 ), ( columns - 1 ) );
                
                unsigned int rowIndex = 0, colIndex = 0; //These need better names
                
                for( unsigned int j = 0; j < rows; j++ )
                {
                    colIndex = 0;
                    
                    for( unsigned int k = 0; k < columns; k++ )
                    {   
                        if( ( j != r ) && ( k != c ) )
                        {
                            _minor( rowIndex, colIndex ) = elements[j][k];
                            colIndex++;
                            
                            if( colIndex >= _minor.columns )
                            {
                                rowIndex++;
                            }
                        }
                    }
                }
                
                //Get determinant of minor, multiply by alternating sign
                matrixOfCoefficients( r, c ) = ( MatrixDeterminant( _minor ) * sign );
                
                //Alternate sign
                sign *= -1;
            }
        }
        
        return ( ( 1 / ( Determinant() ) ) * matrixOfCoefficients.Transposed() );
}

void Matrix::Transpose( void )
{
	( *this ) = Transposed();
}

const Matrix Matrix::Transposed( void )
{
    Matrix temp( columns, rows );

    for( unsigned int i = 0; i < temp.rows; i++ )
	{
		for( unsigned int j = 0; j < temp.columns; j++ )
		{
			temp( i, j ) = elements[j][i];
		}
	}
    
    return temp;
}

double Matrix::Determinant( void )
{
    return MatrixDeterminant( *this );
}

double MatrixDeterminant( const Matrix& mat )
{
    if( !( mat.IsSquare() ) )
    {
        throw invalid_argument( "Matrix is not square" );
    }
    
    if( mat.rows == 1 )
    {
        //If matrix is 1x1, return the only value in the matrix;
        return mat( 0, 0 );
    }
    
    double det = 0.0f;
    int sign = 1;
    
    for( unsigned int i = 0; i < mat.columns; i++ )
    {
        double cofactor = ( mat( 0, i ) * sign );
        
        sign *= -1;
        
        //Build minor - (n-1)x(n-1) matrix
        Matrix _minor( ( mat.rows - 1 ), ( mat.columns - 1 ) );
        
        unsigned int rowIndex = 0, colIndex = 0; //These need better names
        
        for( unsigned int j = 0; j < mat.rows; j++ )
        {
            colIndex = 0;
            
            for( unsigned int k = 0; k < mat.columns; k++ )
            {   
                if( ( j != 0 ) && ( k != i ) )
                {
                    _minor( rowIndex, colIndex ) = mat( j, k );
                    colIndex++;
                    
                    if( colIndex >= _minor.columns )
                    {
                        rowIndex++;
                    }
                }
            }
        }
        
        //Get determinant of minor
        double detMinor = MatrixDeterminant( _minor );
        
        det += ( cofactor * detMinor );
    }
    
    return det;
}

double Matrix::Trace( void )
{
	//This only really makes sense for n x n matrices
    double trace = 0.0f;

    for( unsigned int i = 0; i < rows; i++ )
	{
		trace += elements[i][i];
	}

	return trace;
}

const bool Matrix::IsSquare( void ) const
{
    return ( rows == columns );
}

/////Member Operators/////

Matrix& Matrix::operator=( const Matrix& rhs )
{
    rows = rhs.rows;
    columns = rhs.columns;
    
    elements.clear();
    
    if( ( rows > 0 ) && ( columns > 0 ) )
    {
        elements.resize( rows );
        
        for( unsigned int i = 0; i < rows; i++ )
        {
            elements[i].resize( columns );
        }
        
        for( unsigned int i = 0; i < rows; i++ )
        {
            for( unsigned int j = 0; j < columns; j++ )
            {
                elements[i][j] = rhs( i, j );
            }
        }
    }
    else
    {
        throw invalid_argument( "Matrix cannot have rows/columns less that one" );
    }
    
    return ( *this );
}

Matrix& Matrix::operator*=( const Matrix& rhs )
{
   ( *this ) = ( ( *this ) * rhs );
    
    return ( *this );
}

Matrix& Matrix::operator*=( const double scalar )
{
	for( unsigned int i = 0; i < rows; i++ )
	{
		for( unsigned int j = 0; j < columns; j++ )
		{
			elements[i][j] *= scalar;
		}
	}

	return ( *this );
}

Matrix& Matrix::operator+=( const Matrix& rhs )
{
	for( unsigned int i = 0; i < rows; i++ )
	{
		for( unsigned int j = 0; j < columns; j++ )
		{
			elements[i][j] += rhs( i, j );
		}
	}

	return ( *this );
}

Matrix& Matrix::operator-=( const Matrix& rhs )
{
	for( unsigned int i = 0; i < rows; i++ )
	{
		for( unsigned int j = 0; j < columns; j++ )
		{
			elements[i][j] -= rhs( i, j );
		}
	}

	return ( *this );
}

/////Non-member Operators/////

double& Matrix::operator()( const unsigned int row, const unsigned int column )
{
    if( row >= rows || column >= columns )
    {
        throw invalid_argument( "Row/Column exceeds matrix dimensions" );
    }
    
    return elements[row][column];
}

const double Matrix::operator()( const unsigned int row, const unsigned int column ) const
{
    if( row >= rows || column >= columns )
    {
        throw invalid_argument( "Row/Column exceeds matrix dimensions" );
    }
    
    return elements[row][column];
}

const Matrix operator*( const Matrix& lhs, const Matrix& rhs )
{
    if( lhs.columns != rhs.rows )
    {
        throw invalid_argument( "Incompatible matrices cannot be multiplied" );
    }
    
    Matrix temp( lhs.rows, rhs.columns );
    
    for( unsigned int i = 0; i < lhs.rows; i++ )
    {
        for( unsigned int j = 0; j < lhs.columns; j++ )
        {
            for( unsigned int k = 0; k < lhs.columns; k++ )
            {
                temp( i, j ) += ( lhs( i, k ) * rhs( k, j ) );
            }
        }
    }
    
    return temp;
}

const Matrix operator*( const double scalar, const Matrix& rhs )
{
	Matrix temp( rhs );

	return ( temp *= scalar );
}

const Matrix operator*( const Matrix& lhs, const double scalar )
{
	Matrix temp( lhs );

	return ( temp *= scalar );
}

const Matrix operator+( const Matrix& lhs, const Matrix& rhs )
{
	Matrix temp( lhs );

	return ( temp += rhs );
}

const Matrix operator-( const Matrix& lhs, const Matrix& rhs )
{
	Matrix temp( lhs );

	return ( temp -= rhs );
}

std::ostream& operator<<(std::ostream& os, const Matrix& mat)
{
    for( unsigned int i = 0; i < mat.rows; i++ )
    {
        for( unsigned int j = 0; j < mat.columns; j++ )
        {
            os << mat( i, j ) << " ";
        }
        os << std::endl;
    }
    return os;
}