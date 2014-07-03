#include "Maths/Vector.h"

Vector2D::Vector2D( void ) : x( 0.0f ), y( 0.0f )
{
}

Vector2D::Vector2D( double _x, double _y ) : x( _x ), y( _y )
{
}

Vector2D::~Vector2D( void )
{
}

double Vector2D::Magnitude( void )
{
	return sqrt( ( x * x ) + ( y * y ) );
}

void Vector2D::Normalise( void )
{
	double t = Magnitude();
    
    x /= t;
    y /= t;
}

const Vector2D Vector2D::Normalised( void )
{
	Vector2D temp( *this );

	double t = Magnitude();
    
    temp.x /= t;
    temp.y /= t;

    return temp;
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

const Vector2D Vector2D::operator+( const Vector2D& rhs )
{
	return Vector2D( *this ) += rhs;
}

const Vector2D Vector2D::operator-( const Vector2D& rhs )
{
	return Vector2D( *this ) -= rhs;
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

const Vector2D operator*( const double& scalar, const Vector2D& a )
{
	return Vector2D( a ) *= scalar;
}

const Vector2D operator*( const Vector2D& a, const double& scalar )
{
	return ( scalar * a );
}

double DotProduct( Vector2D a, Vector2D b )
{
	return ( ( a.x * b.x ) + ( a.y * b.y ) );
}

std::ostream& operator<<( std::ostream& os, const Vector2D& a )
{
	os << a.x << " " << a.y;
    
    return os;
}