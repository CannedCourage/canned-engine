#include "Physics/CollisionAreas.h"
#include <cmath>

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

//OBB//
OBB::OBB( void )
{
	SetRotation( 0 );
}

void OBB::SetRotation( double Angle )
{
	Rotation = Angle;

	double _sin = std::sin( Angle );
	double _cos = std::cos( Angle );

	RotatedXAxis.x = _cos;
	RotatedXAxis.y = _sin;

	RotatedYAxis.x = -_sin;
	RotatedYAxis.y = _cos;

	RotatedXAxis.Normalise();
	RotatedYAxis.Normalise();
}

const Vector2D& OBB::LocalX( void ) const
{
	return RotatedXAxis;
}

const Vector2D& OBB::LocalY( void ) const
{
	return RotatedYAxis;
}