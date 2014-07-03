#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <iostream>
#include <math.h>

struct D3DXVECTOR2;
struct D3DXVECTOR3;
struct D3DXVECTOR4;

class Vector2D
{
private:
protected:
public:

	double x, y;

	Vector2D( void );
	Vector2D( double _x, double _y );
	~Vector2D( void );

	double Magnitude( void );

	void Normalise( void );
	const Vector2D Normalised( void );

	Vector2D& operator+=( const Vector2D& rhs );
	Vector2D& operator-=( const Vector2D& rhs );
	const Vector2D operator+( const Vector2D& rhs );
	const Vector2D operator-( const Vector2D& rhs );
	Vector2D& operator*=( const double& scalar );
	bool operator==( const Vector2D &rhs ) const;
	bool operator!=( const Vector2D &rhs ) const;

	D3DXVECTOR2 ToD3D( void );
};

const Vector2D operator*( const double& scalar, const Vector2D& a );
const Vector2D operator*( const Vector2D& a, const double& scalar );

double DotProduct( Vector2D a, Vector2D b );

std::ostream& operator<<( std::ostream& os, const Vector2D& a );

class Vector3D
{
private:
protected:
public:

	double x, y, z;

	Vector3D( void );
	Vector3D( double _x, double _y, double _z );
	~Vector3D( void );;;

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

	double x, y, z, w;

	Vector4D( void );
	Vector4D( double _x, double _y, double _z, double _w );
	~Vector4D( void );;;

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

#endif //_VECTOR_H_