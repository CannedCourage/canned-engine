#include "Maths/Vector.h"
#include <math.h>

Vector::Vector( void ) : x( 0.0f ), y( 0.0f ), z( 0.0f )
{
}

Vector::Vector( const float _x, const float _y, const float _z ) : x( _x ), y( _y ), z( _z )
{
}

Vector::~Vector( void )
{
}

float Vector::DotProduct( const Vector& rhs )
{
	float temp = 0.0f;

	temp += ( x * rhs.x );
	temp += ( y * rhs.y );
	temp += ( z * rhs.z );

	return temp;
}

Vector Vector::CrossProduct( const Vector& rhs )
{
	Vector temp;

	temp.x = ( (y*rhs.z) - (z*rhs.y) );
	temp.y = ( (x*rhs.z) - (z*rhs.x) );
	temp.z = ( (x*rhs.y) - (y*rhs.x) );

	return temp;
}

void Vector::Clear( void )
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

void Vector::Normalize( void )
{
	float s = (x*x) + (y*y) + (z*z);
	s = sqrt( s );

	x /= s;
	y /= s;
	z /= s;
}

float Vector::SqrMagnitude( void )
{
	return ( ( x * x ) + ( y * y ) + ( z * z ) );
}

float Vector::Magnitude( void )
{
	return sqrtf( SqrMagnitude() );
}

Vector& Vector::operator=( const Vector& rhs )
{
	if( this != &rhs )
	{
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
	}

	return *this;
}

bool Vector::operator==( const Vector& rhs ) const
{
	if( (this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Vector::operator!=( const Vector& rhs ) const
{
	return !( (*this) == rhs );
}

Vector& Vector::operator+=( const Vector& rhs )
{
	if( this != &rhs )
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
	}

	return *this;
}

Vector& Vector::operator-=( const Vector& rhs )
{
	if( this != &rhs )
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
	}

	return *this;
}

Vector Vector::operator+( const Vector& rhs ) const
{
	return ( Vector( *this ) += rhs );
}

Vector Vector::operator-( const Vector& rhs ) const
{
	return ( Vector( *this ) -= rhs );
}

Vector& Vector::operator*=( const float& rhs )
{
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;

	return (*this);
}

Vector& Vector::operator/=( const float& rhs )
{
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;

	return (*this);
}

Vector Vector::operator*( const float& rhs ) const
{
	return ( Vector( *this ) *= rhs );
}

Vector operator*( const float& lhs, const Vector& rhs )
{
	return ( Vector( rhs ) *= lhs );
}

Vector Vector::operator/( const float& rhs ) const
{
	return ( Vector( *this ) /= rhs );
}