#include "Maths/Vector.h"

Vector4D::Vector4D( void ) : x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f )
{
}

Vector4D::Vector4D( double _x, double _y, double _z, double _w ) : x( _x ), y( _y ), z( _z ), w( _w )
{
}

Vector4D::~Vector4D( void )
{
}

double Vector4D::Magnitude( void )
{
    return sqrt( ( x * x ) + ( y * y ) + ( z * z ) + ( w * w ) );
}

void Vector4D::Normalise( void )
{
    double t = Magnitude();
    
    x /= t;
    y /= t;
    z /= t;
    w /= t;
}

const Vector4D Vector4D::Normalised( void )
{   
    Vector4D temp( *this );
    
    double t = Magnitude();
    
    temp.x /= t;
    temp.y /= t;
    temp.z /= t;
    temp.w /= t;
    
    return temp;
}

Vector4D& Vector4D::operator+=( const Vector4D& rhs )
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    
    return ( *this );
}

Vector4D& Vector4D::operator-=( const Vector4D& rhs )
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    
    return ( *this );
}

const Vector4D Vector4D::operator+( const Vector4D& rhs )
{
    return Vector4D( *this ) += rhs;
}

const Vector4D Vector4D::operator-( const Vector4D& rhs )
{
    return Vector4D( *this ) -= rhs;
}

Vector4D& Vector4D::operator*=( const double& scalar )
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    
    return ( *this );
}

bool Vector4D::operator==( const Vector4D &rhs ) const
{
    // Compare the values, and return a bool result.
    if( ( x == rhs.x ) && ( y == rhs.y ) && ( z == rhs.z ) && ( w == rhs.w ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Vector4D::operator!=( const Vector4D &rhs ) const
{
    return !( *this == rhs );
}

const Vector4D operator*( const double& scalar, const Vector4D& a )
{
    return Vector4D( a ) *= scalar;
}

const Vector4D operator*( const Vector4D& a, const double& scalar )
{
    return ( scalar * a );
}

double DotProduct( Vector4D a, Vector4D b )
{
    return ( ( a.x * b.x ) + ( a.y * b.y ) + ( a.z * b.z ) + ( a.w + b.w ) );
}

std::ostream& operator<<( std::ostream& os, const Vector4D& a )
{
    os << a.x << " " << a.y << " " << a.z;
    
    return os;
}