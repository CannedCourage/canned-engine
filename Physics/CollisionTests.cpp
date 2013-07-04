#include "Physics\CollisionTests.h"
#include "Maths\Clamp.h"

//ADD CONST CORRECTNESS

//Tests for intersection, etc. between circle and AABB
//Based on posts on Stack Overflow: http://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
bool isIntersecting( BoundingCircle circle, BoundingRectangle rect, Vector* closestPointOnRectangle )
{
	// Find the closest point to the circle within the rectangle
	float closestX = clamp( circle.centre().x, rect.left(), rect.right() );
	float closestY = clamp( circle.centre().y, rect.bottom(), rect.top() );

	closestPointOnRectangle->x = closestX;
	closestPointOnRectangle->y = closestY;

	// Calculate the distance between the circle's center and this closest point
	float distanceX = circle.centre().x - closestX;
	float distanceY = circle.centre().y - closestY;

	// If the distance is less than the circle's radius, an intersection occurs
	float distanceSquared = ( distanceX * distanceX ) + ( distanceY * distanceY );
	return ( distanceSquared <= ( circle.radius() * circle.radius() ) );
}

//Is the point inside or on the rectangle, or not?
bool isIntersecting( Vector point, BoundingRectangle rect )
{
	if( ( point.x >= rect.left() ) && ( point.x <= rect.right() ) && ( point.y >= rect.bottom() ) && ( point.y <= rect.top() ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isIntersecting( BoundingRectangle a, BoundingRectangle b )
{
	if( a.right() < b.left() || a.left() > b.right() )
	{
		return false;
	}

	if( a.top() < b.bottom() || a.bottom() > b.top() )
	{
		return false;
	}
                
    return true;
}

Vector relativePosition( BoundingCircle circle, BoundingRectangle rect )
{
	Vector result( 0, 0, 0 );

	if( circle.centre().x <= rect.left() )
	{
		result.x = -1;
	}

	if( circle.centre().x >= rect.right() )
	{
		result.x = 1;
	}

	if( circle.centre().y >= rect.top() )
	{
		result.y = 1;
	}

	if( circle.centre().y <= rect.bottom() )
	{
		result.y = -1;
	}

	return result;
}

Vector relativePosition( BoundingRectangle rect, BoundingCircle circle )
{
	return relativePosition( circle, rect );
}