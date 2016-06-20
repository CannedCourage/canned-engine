#include "Physics/CollisionAreas.h"
#include "Physics/CollisionAreaTests.h"

#include <cmath>

using namespace Collisions2D;

//DISTANCE//
float DistanceSquaredPointToAABB( const Point2D& Point, const AABB& Box )
{
	float distanceSquared = 0.0f;

	if( Point.x < Box.MinX() )
	{
		distanceSquared += ( ( Box.MinX() - Point.x ) * ( Box.MinX() - Point.x ) );
	}

	if( Point.x > Box.MaxX() )
	{
		distanceSquared += ( ( Point.x - Box.MaxX() ) * ( Point.x - Box.MaxX() ) );
	}

	if( Point.y < Box.MinY() )
	{
		distanceSquared += ( ( Box.MinY() - Point.y ) * ( Box.MinY() - Point.y ) );
	}

	if( Point.y > Box.MaxY() )
	{
		distanceSquared += ( ( Point.y - Box.MaxY() ) * ( Point.y - Box.MaxY() ) );
	}

	return distanceSquared;
}

//CIRCLE VS X//
bool IntersectionTest( const Circle& A, const Circle& B )
{
	//Determine the vector between the two centres
	Vector2D vectorBetweenCentres = A.Centre - B.Centre;

	//Circles intersect if the distance (squared) between the centres
	//is less than or equal to the sum of the radii (squared)
	float sumOfRadii = A.Radius + B.Radius;

	return ( vectorBetweenCentres.MagnitudeSquared() <= ( sumOfRadii * sumOfRadii ) );
}

bool IntersectionTest( const Circle& A, const AABB& B )
{
	//Calculate shortest distance from Circle centre to AABB
	float distanceSquared = DistanceSquaredPointToAABB( A.Centre, B );

	//If this distance (squared) is shorter than the Circle radius (squared)
	//then the Circle and AABB intersect
	return ( distanceSquared <= ( A.Radius * A.Radius ) );
}

//AABB VS X//
bool IntersectionTest( const AABB& A, const AABB& B )
{
	//Seperating Axis Theorem

	//If the x component of distance between the centres
	//is greater than the sum of the halfwidths along the x-axis of each AABB
	//then the AABBs do not intersect
	if( std::abs( A.Centre.x - B.Centre.x ) > ( A.Halfwidth.x + B.Halfwidth.x ) )
	{
		return false;
	}

	//If the y component of distance between the centres
	//is greater than the sum of the halfwidths along the y-axis of each AABB
	//then the AABBs do not intersect
	if( std::abs( A.Centre.y - B.Centre.y ) > ( B.Halfwidth.y + B.Halfwidth.y ) )
	{
		return false;
	}

	return true;
}

bool IntersectionTest( const AABB& A, const Circle& B )
{
	return IntersectionTest( B, A );
}