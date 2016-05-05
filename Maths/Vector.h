#ifndef _VECTOR_H_
#define _VECTOR_H_

//TODO: Sort out return values for vector methods, should be const reference?
//TODO: Check constness of overloaded operators
//TODO: Move binary arithmetic operator overloads to be non-members
//TODO: Style update, capitalised members + parameters

#include <iostream>

struct D3DXVECTOR2;
struct D3DXVECTOR3;
struct D3DXVECTOR4;

class Vector2D
{
private:
protected:
public:

	double x = 0.0f, y = 0.0f;

	Vector2D( void ) = default;
	Vector2D( double _x, double _y );
	~Vector2D( void ) = default;

	double MagnitudeSquared( void ) const;
	double Magnitude( void ) const;

	void Normalise( void );
	Vector2D Normalised( void );

	Vector2D LeftNormal( void );
	Vector2D RightNormal( void );

	Vector2D& operator+=( const Vector2D& rhs );
	Vector2D& operator-=( const Vector2D& rhs );
	Vector2D& operator*=( const double& scalar );
	bool operator==( const Vector2D &rhs ) const;
	bool operator!=( const Vector2D &rhs ) const;

	D3DXVECTOR2 ToD3D( void );
};

Vector2D operator+( const Vector2D& lhs, const Vector2D& rhs );
Vector2D operator-( const Vector2D& lhs, const Vector2D& rhs );
Vector2D operator*( const double& scalar, const Vector2D& a );
Vector2D operator*( const Vector2D& a, const double& scalar );

double DotProduct( const Vector2D& a, const Vector2D& b );

Vector2D ProjectVector( const Vector2D& A, const Vector2D& B );

std::ostream& operator<<( std::ostream& os, const Vector2D& a );

class Vector3D
{
private:
protected:
public:

	double x = 0.0f, y = 0.0f, z = 0.0f;

	Vector3D( void );
	Vector3D( double _x, double _y, double _z );
	~Vector3D( void );

	double Magnitude( void );

	void Normalise( void );
	const Vector3D Normalised( void );

	Vector3D& operator+=( const Vector3D& rhs );
	Vector3D& operator-=( const Vector3D& rhs );
	const Vector3D operator+( const Vector3D& rhs );
	const Vector3D operator-( const Vector3D& rhs );
	Vector3D& operator*=( const double& scalar );
	bool operator==( const Vector3D &rhs ) const;
	bool operator!=( const Vector3D &rhs ) const;

	D3DXVECTOR3 ToD3D( void );
};

const Vector3D operator*( const double& scalar, const Vector3D& a );
const Vector3D operator*( const Vector3D& a, const double& scalar );

double DotProduct( Vector3D a, Vector3D b );
const Vector3D CrossProduct( Vector3D a, Vector3D b );

std::ostream& operator<<( std::ostream& os, const Vector3D& a );

class Vector4D
{
private:
protected:
public:

	double x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

	Vector4D( void );
	Vector4D( double _x, double _y, double _z, double _w );
	~Vector4D( void );

	double Magnitude( void );

	void Normalise( void );
	const Vector4D Normalised( void );

	Vector4D& operator+=( const Vector4D& rhs );
	Vector4D& operator-=( const Vector4D& rhs );
	const Vector4D operator+( const Vector4D& rhs );
	const Vector4D operator-( const Vector4D& rhs );
	Vector4D& operator*=( const double& scalar );
	bool operator==( const Vector4D &rhs ) const;
	bool operator!=( const Vector4D &rhs ) const;

	D3DXVECTOR4 ToD3D( void );
};

const Vector3D operator*( const double& scalar, const Vector3D& a );
const Vector3D operator*( const Vector3D& a, const double& scalar );

double DotProduct( Vector3D a, Vector3D b );

std::ostream& operator<<( std::ostream& os, const Vector3D& a );

typedef Vector2D Point2D;
typedef Vector3D Point3D;
typedef Vector4D Point4D;

#endif //_VECTOR_H_