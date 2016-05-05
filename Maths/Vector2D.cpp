#include "Maths/Vector.h"
#include <cmath>

Vector2D::Vector2D( double _x, double _y ) : x( _x ), y( _y )
{
}

double Vector2D::MagnitudeSquared( void ) const
{
    return ( ( x * x ) + ( y * y ) );
}

double Vector2D::Magnitude( void ) const
{
	return std::sqrt( ( x * x ) + ( y * y ) );
}

void Vector2D::Normalise( void )
{
	double t = Magnitude();
    
    x /= t;
    y /= t;
}

Vector2D Vector2D::Normalised( void )
{
	Vector2D temp( *this );

	temp.Normalise();

    return temp;
}

Vector2D Vector2D::LeftNormal( void )
{
    return Vector2D( -y, x );
}

Vector2D Vector2D::RightNormal( void )
{
    return Vector2D( y, -x );
}

Vector2D& Vector2D::operator+=( const Vector2D& rhs )
{
	x += rhs.x;
    y += rhs.y;
    
    return ( *this );
}

Vector2D& Vector2D::operator-=( const Vector2D& rhs )
{
	x -= rhs.x;
    y -= rhs.y;
    
    return ( *this );
}

Vector2D& Vector2D::operator*=( const double& scalar )
{
	x *= scalar;
    y *= scalar;
    
    return ( *this );
}

bool Vector2D::operator==( const Vector2D &rhs ) const
{
	// Compare the values, and return a bool result.
    if( ( x == rhs.x ) && ( y == rhs.y ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Vector2D::operator!=( const Vector2D &rhs ) const
{
	return !( *this == rhs );
}

Vector2D operator+( const Vector2D& lhs, const Vector2D& rhs )
{
    return ( Vector2D( lhs ) += rhs );
}

Vector2D operator-( const Vector2D& lhs, const Vector2D& rhs )
{
    return ( Vector2D( lhs ) -= rhs );
}

Vector2D operator*( const double& scalar, const Vector2D& a )
{
	return ( Vector2D( a ) *= scalar );
}

Vector2D operator*( const Vector2D& a, const double& scalar )
{
	return ( Vector2D( a ) *= scalar );
}

double DotProduct( const Vector2D& a, const Vector2D& b )
{
	return ( ( a.x * b.x ) + ( a.y * b.y ) );
}

Vector2D ProjectVector( const Vector2D& A, const Vector2D& B )
{
    //Scalar Projection = |A|cos(Theta)
    //OR
    //Scalar Projection = (A.B) / |B|
    double scalarProjection = ( DotProduct( A, B ) / B.MagnitudeSquared() );

    //Vector Projection = Scalar Projection * B
    Vector2D vectorProjection;

    vectorProjection.x = ( scalarProjection * B.x );
    vectorProjection.y = ( scalarProjection * B.y );

    return vectorProjection;
}

std::ostream& operator<<( std::ostream& os, const Vector2D& a )
{
	os << a.x << " " << a.y;
    
    return os;
}