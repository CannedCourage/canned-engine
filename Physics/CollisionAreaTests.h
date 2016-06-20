#ifndef _COLLISIONAREATESTS_H_
#define _COLLISIONAREATESTS_H_

#include "Maths/Vector.h"

//Cannot forward declare "Point2D" because it is a typedef
//Included Vector.h for completeness

namespace Collision2D
{
	class Circle;
	class Capsule;
	class AABB;
	class OBB;

	//DISTANCE//
	float DistanceSquaredPointToAABB( const Point2D& Point, const AABB& Box );

	//CLOSEST POINT//

	//CIRCLE VS X//
	bool IntersectionTest( const Circle& A, const Circle& B );
	bool IntersectionTest( const Circle& A, const Capsule& B );
	bool IntersectionTest( const Circle& A, const AABB& B );
	bool IntersectionTest( const Circle& A, const OBB& B );

	//CAPSULE VS X//
	bool IntersectionTest( const Capsule& A, const Capsule& B );
	bool IntersectionTest( const Capsule& A, const Circle& B );
	bool IntersectionTest( const Capsule& A, const AABB& B );
	bool IntersectionTest( const Capsule& A, const OBB& B );

	//AABB VS X//
	bool IntersectionTest( const AABB& A, const AABB& B );
	bool IntersectionTest( const AABB& A, const Circle& B );
	bool IntersectionTest( const AABB& A, const Capsule& B );
	bool IntersectionTest( const AABB& A, const OBB& B );

	//OBB VS X//
	bool IntersectionTest( const OBB& A, const OBB& B );
	bool IntersectionTest( const OBB& A, const Circle& B );
	bool IntersectionTest( const OBB& A, const Capsule& B );
	bool IntersectionTest( const OBB& A, const AABB& B );
} 

#endif //_COLLISIONAREATESTS_H_