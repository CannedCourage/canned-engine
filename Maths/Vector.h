#ifndef _VECTOR_H_
#define _VECTOR_H_

class Vector
{
public:

	float x, y, z;

	Vector( void );
	Vector( const float _x, const float _y, const float _z );
	~Vector( void );

	float DotProduct( const Vector& rhs );
	Vector CrossProduct( const Vector& rhs );
	void Clear( void );
	void Normalize( void );
	float SqrMagnitude( void );
	float Magnitude( void );

	Vector& operator=( const Vector& rhs );

	bool operator==( const Vector& rhs ) const;

	bool operator!=( const Vector& rhs ) const;

	Vector& operator+=( const Vector& rhs );

	Vector& operator-=( const Vector& rhs );

	Vector operator+( const Vector& rhs ) const;

	Vector operator-( const Vector& rhs ) const;

	Vector& operator*=( const float& rhs );

	Vector& operator/=( const float& rhs );

	Vector operator*( const float& rhs ) const;

	Vector operator/( const float& rhs ) const;
};

Vector operator*( const float& lhs, const Vector& rhs );

#endif //_VECTOR_H_