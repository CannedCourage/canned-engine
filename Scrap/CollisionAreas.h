#ifndef _COLLISION_AREAS_H_
#define _COLLISION_AREAS_H_

#include "Maths/Vector.h"

class BoundingCircle
{
private:
protected:

	Vector Centre;
	float Radius;
public:

	BoundingCircle( void );
	~BoundingCircle( void );

	Vector& centre( void );
	float& radius( void );
	//Add const functions
};

class BoundingRectangle
{
private:
protected:

	float Left, Right, Top, Bottom;
public:

	BoundingRectangle( void );
	~BoundingRectangle( void );

	float& left( void );
	float& right( void );
	float& top( void );
	float& bottom( void );

	Vector TopLeft( void );
	Vector TopRight( void );
	Vector BottomLeft( void );
	Vector BottomRight( void );

	//Add const functions
};

#endif //_COLLISION_AREAS_H_