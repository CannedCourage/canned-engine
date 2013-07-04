#include "Maths\Clamp.h"

int clamp( const int value, const int min, const int max )
{
	if( value < min )
	{
		return min;
	}

	if( value > max )
	{
		return max;
	}

	return value;
}

float clamp( const float value, const float min, const float max )
{
	if( value < min )
	{
		return min;
	}

	if( value > max )
	{
		return max;
	}

	return value;
}