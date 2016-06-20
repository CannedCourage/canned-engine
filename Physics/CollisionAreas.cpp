#include "Physics/CollisionAreas.h"

using namespace Collisions2D;

//AABB//
double AABB::MinX( void ) const
{
	return ( Centre.x - Halfwidth.x );
}

double AABB::MaxX( void ) const
{
	return ( Centre.x + Halfwidth.x );
}

double AABB::MinY( void ) const
{
	return ( Centre.y - Halfwidth.y );
}

double AABB::MaxY( void ) const
{
	return ( Centre.y + Halfwidth.y );
}

//CAPSULE//
Vector2D Capsule::MedialAxis( void )
{
	Vector2D medialAxis{};

	return medialAxis;
}