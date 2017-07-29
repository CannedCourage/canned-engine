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
		double Radius = 0.0;

		Circle( void ) = default;
		~Circle( void ) = default;
	};

	class Capsule
	{
	private:
	protected:

	public:
		Point2D CenterA, CentreB;
		double Radius = 0.0;

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

		double Rotation = 0.0;
		Vector2D RotatedXAxis, RotatedYAxis;
	public:

		Point2D Centre;
		Vector2D Halfwidth;

		OBB( void );
		~OBB( void ) = default;

		void SetRotation( double Angle );
		const Vector2D& LocalX( void ) const;
		const Vector2D& LocalY( void ) const;

		//TODO: Add methods to return maxX = centre + ( halfwidth.x * RotatedXAxis ) etc.
	};
}

#endif //_COLLISIONAREAS_H_