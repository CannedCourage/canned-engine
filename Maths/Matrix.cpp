#include "Maths/Matrix.h"
using std::invalid_argument;

Matrix::Matrix( const int r, const int c ) : rows( r ), columns( c )
{
    if( ( rows > 0 ) && ( columns > 0 ) )
    {
        elements.resize( rows );
        
        for( int i = 0; i < rows; i++ )
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
        
        for( int i = 0; i < rows; i++ )
        {
            elements[i].resize( columns );
        }
        
        for( int i = 0; i < rows; i++ )
        {
            for( int j = 0; j < columns; j++ )
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

	for( int i = 0; i < rows; i++ )
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
    Matrix temp;
    
    return temp;
}

void Matrix::Transpose( void )
{
	( *this ) = Transposed();
}

const Matrix Matrix::Transposed( void )
{
    Matrix temp( columns, rows );

    for( int i = 0; i < temp.rows; i++ )
	{
		for( int j = 0; j < temp.columns; j++ )
		{
			temp( i, j ) = elements[j][i];
		}
	}
    
    return temp;
}

double Matrix::Determinant( void )
{
    return 0.0f;
}

double Matrix::Trace( void )
{
	//This only really makes sense for n x n matrices

    double trace = 0.0f;

    for( int i = 0; i < rows; i++ )
	{
		trace += elements[i][i];
	}
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
        
        for( int i = 0; i < rows; i++ )
        {
            elements[i].resize( columns );
        }
        
        for( int i = 0; i < rows; i++ )
        {
            for( int j = 0; j < columns; j++ )
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
	for( int i = 0; i < rows; i++ )
	{
		for( int j = 0; j < columns; j++ )
		{
			elements[i][j] *= scalar;
		}
	}
}

Matrix& Matrix::operator+=( const Matrix& rhs )
{
	for( int i = 0; i < rows; i++ )
	{
		for( int j = 0; j < columns; j++ )
		{
			elements[i][j] += rhs( i, j );
		}
	}
}

Matrix& Matrix::operator-=( const Matrix& rhs )
{
	for( int i = 0; i < rows; i++ )
	{
		for( int j = 0; j < columns; j++ )
		{
			elements[i][j] -= rhs( i, j );
		}
	}
}

/////Non-member Operators/////

double& Matrix::operator()( const unsigned int row, const unsigned int column )
{
    return elements[row][column];
}

const double Matrix::operator()( const unsigned int row, const unsigned int column ) const
{
    return elements[row][column];
}

const Matrix operator*( const Matrix& lhs, const Matrix& rhs )
{
    if( lhs.columns != rhs.rows )
    {
        throw invalid_argument( "Incompatible matrices cannot be multiplied" );
    }
    
    Matrix temp( lhs.rows, rhs.columns );
    
    for( int i = 0; i < lhs.rows; i++ )
    {
        for( int j = 0; j < lhs.columns; j++ )
        {
            for( int k = 0; k < lhs.columns; k++ )
            {
                temp( i, j ) += ( lhs( i, k ) * rhs( k, j ) );
            }
        }
    }
    
    return temp;
}

const Matrix operator*( const double scalar, const Matrix& rhs )
{
	return ( Matrix temp( rhs ) *= scalar );
}

const Matrix operator*( const Matrix& lhs, const double scalar )
{
	return ( Matrix temp( lhs ) *= scalar );
}

const Matrix operator+( const Matrix& lhs, const Matrix& rhs )
{
	return ( Matrix temp( lhs ) += rhs );
}

const Matrix operator-( const Matrix& lhs, const Matrix& rhs )
{
	return ( Matrix temp( lhs ) -= rhs );
}