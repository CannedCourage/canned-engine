#include "Maths/Vector.h"

Vector3D::Vector3D( void ) : x( 0.0f ), y( 0.0f ), z( 0.0f )
{
}

Vector3D::Vector3D( double _x, double _y, double _z ) : x( _x ), y( _y ), z( _z )
{
}

Vector3D::~Vector3D( void )
{
}

double Vector3D::Magnitude( void )
{
    return sqrt( ( x * x ) + ( y * y ) + ( z * z ) );
}

void Vector3D::Normalise( void )
{
    double t = Magnitude();
    
    x /= t;
    y /= t;
    z /= t;
}

const Vector3D Vector3D::Normalised( void )
{   
    Vector3D temp( *this );
    
    double t = Magnitude();
    
    temp.x /= t;
    temp.y /= t;
    temp.z /= t;
    
    return temp;
}

Vector3D& Vector3D::operator+=( const Vector3D& rhs )
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    
    return ( *this );
}

Vector3D& Vector3D::operator-=( const Vector3D& rhs )
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    
    return ( *this );
}

const Vector3D Vector3D::operator+( const Vector3D& rhs )
{
    return Vector3D( *this ) += rhs;
}

const Vector3D Vector3D::operator-( const Vector3D& rhs )
{
    return Vector3D( *this ) -= rhs;
}

Vector3D& Vector3D::operator*=( const double& scalar )
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    
    return ( *this );
}

bool Vector3D::operator==( const Vector3D &rhs ) const
{
    // Compare the values, and return a bool result.
    if( ( x == rhs.x ) && ( y == rhs.y ) && ( z == rhs.z ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Vector3D::operator!=( const Vector3D &rhs ) const
{
    return !( *this == rhs );
}

const Vector3D operator*( const double& scalar, const Vector3D& a )
{
    return Vector3D( a ) *= scalar;
}

const Vector3D operator*( const Vector3D& a, const double& scalar )
{
    return ( scalar * a );
}

double DotProduct( Vector3D a, Vector3D b )
{
    return ( ( a.x * b.x ) + ( a.y * b.y ) + ( a.z * b.z ) );
}

const Vector3D CrossProduct( Vector3D a, Vector3D b )
{
    Vector3D c;
    
    c.x = ( ( a.y * b.z ) - ( b.y * a.z ) );
    c.y = ( ( a.z * b.x ) - ( b.z * a.x ) );
    c.z = ( ( a.x * b.y ) - ( b.x * a.y ) );
    
    return c;
}

std::ostream& operator<<( std::ostream& os, const Vector3D& a )
{
    os << a.x << " " << a.y << " " << a.z;
    
    return os;
}