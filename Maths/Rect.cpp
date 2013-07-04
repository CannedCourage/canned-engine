#include "Maths\Rect.h"

Rect::Rect( void )
{
}

Rect::~Rect( void )
{
}

Vector Rect::topLeft( void )
{
	return Vector( left, top, 0 );
}

Vector Rect::bottomLeft( void )
{
	return Vector( left, bottom, 0 );
}

Vector Rect::topRight( void )
{
	return Vector( right, top, 0 );
}

Vector Rect::bottomRight( void )
{
	return Vector( right, bottom, 0 );
}