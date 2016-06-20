#ifndef _COLLISIONAREAS_H_
#define _COLLISIONAREAS_H_

#include "Maths/Vector.h"

namespace Collisions2D
{
	class Circle
	{
	private:
	protected:

	public:
		Point2D Centre;
		double Radius = 0.0f;

		Circle( void ) = default;
		~Circle( void ) = default;
	};

	class Capsule
	{
	private:
	protected:

	public:
		Point2D CenterA, CentreB;
		double Radius = 0.0f;

		Capsule( void ) = default;
		~Capsule( void ) = default;

		Vector2D MedialAxis( void );
	};

	class AABB
	{
	private:
	protected:

	public:
		Point2D Centre;
		Vector2D Halfwidth;

		AABB( void ) = default;
		~AABB( void ) = default;

		double MinX( void ) const;
		double MaxX( void ) const;
		
		double MinY( void ) const;
		double MaxY( void ) const;
	};

	class OBB
	{
	private:
	protected:

	public:
		Point2D Centre;
		Vector2D Halfwidth;
		double Rotation; //Should this be a direction vector?

		OBB( void ) = default;
		~OBB( void ) = default;
	};
}

#endif //_COLLISIONAREAS_H_