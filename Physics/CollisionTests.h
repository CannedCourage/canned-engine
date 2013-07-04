#ifndef _COLLISION_TESTS_H_
#define _COLLISION_TESTS_H_

//ADD CONST CORRECTNESS

#include "CollisionAreas.h"

bool isIntersecting( BoundingCircle circle, BoundingRectangle rect, Vector* closestPointOnRectangle );

bool isIntersecting( Vector point, BoundingRectangle rect );

bool isIntersecting( BoundingRectangle a, BoundingRectangle b );

Vector relativePosition( BoundingCircle circle, BoundingRectangle rect );
Vector relativePosition( BoundingRectangle rect, BoundingCircle circle );

#endif //_COLLISION_TESTS_H_