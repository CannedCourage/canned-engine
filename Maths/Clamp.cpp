#include "Maths/Clamp.h"
/*
int clamp( int n, int minimum, int maximum )
{
	if( n < minimum ){ return minimum; }
	if( n > maximum ){ return maximum; }

	return n;
}

float clamp( float n, float minimum, float maximum )
{
	if( n < minimum ){ return minimum; }
	if( n > maximum ){ return maximum; }

	return n;
}
//*/
double clamp( double n, double minimum, double maximum )
{
	if( n < minimum ){ return minimum; }
	if( n > maximum ){ return maximum; }

	return n;
}
/*
int min( int n, int minimum )
{
	if( n < minimum )
	{
		return minimum;
	}

	return n;
}

float min( float n, float minimum )
{
	if( n < minimum )
	{
		return minimum;
	}

	return n;
}
//*/
double min( double n, double minimum )
{
	if( n < minimum )
	{
		return minimum;
	}

	return n;
}
/*
int max( int n, int maximum )
{
	if( n > maximum )
	{
		return maximum;
	}

	return n;
}

float max( float n, float maximum )
{
	if( n > maximum )
	{
		return maximum;
	}

	return n;
}
//*/
double max( double n, double maximum )
{
	if( n > maximum )
	{
		return maximum;
	}

	return n;
}
/*
int most( int a, int b )
{
	if( a > b ){ return a; }
	else{ return b; }
}

float most( float a, float b )
{
	if( a > b ){ return a; }
	else{ return b; }
}
//*/
double most( double a, double b )
{
	if( a > b ){ return a; }
	else{ return b; }
}
/*
int least( int a, int b )
{
	if( a < b ){ return a; }
	else{ return b; }
}

float least( float a, float b )
{
	if( a < b ){ return a; }
	else{ return b; }
}
//*/
double least( double a, double b )
{
	if( a < b ){ return a; }
	else{ return b; }
}
