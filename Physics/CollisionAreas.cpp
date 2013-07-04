#include "Physics\CollisionAreas.h"

BoundingCircle::BoundingCircle( void )
{
	Radius = 1;
	Centre.x = 0;
	Centre.y = 0;
}

BoundingCircle::~BoundingCircle( void )
{
}

Vector& BoundingCircle::centre( void )
{
	return Centre;
}

float& BoundingCircle::radius( void )
{
	return Radius;
}

BoundingRectangle::BoundingRectangle( void )
{
}

BoundingRectangle::~BoundingRectangle( void )
{
}

float& BoundingRectangle::left( void )
{
	return Left;
}

float& BoundingRectangle::right( void )
{
	return Right;
}

float& BoundingRectangle::top( void )
{
	return Top;
}

float& BoundingRectangle::bottom( void )
{
	return Bottom;
}

Vector BoundingRectangle::TopLeft( void )
{
	Vector temp( Top, Left, 0 );
	return temp;
}

Vector BoundingRectangle::TopRight( void )
{
	Vector temp( Top, Right, 0 );
	return temp;
}

Vector BoundingRectangle::BottomLeft( void )
{
	Vector temp( Bottom, Left, 0 );
	return temp;
}

Vector BoundingRectangle::BottomRight( void )
{
	Vector temp( Bottom, Right, 0 );
	return temp;
}