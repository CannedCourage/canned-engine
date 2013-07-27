#include "Engine/Time.h"
#include "Maths/Clamp.h"

Time::Time( void ) : begin( ), end( ), freq( 0.0f ), deltaTime( 0.0f ), fixedStep( 0.01f ), timeScaleGlobal( 1.0f ), physicsAccumulator( 0.0f ), MAXDT( 0.25 )
{
	freq = getFreq();
}

Time::~Time( void )
{
}

double Time::getFreq( void )
{
	LARGE_INTEGER proc_freq;

	if ( !::QueryPerformanceFrequency( &proc_freq ) )
		throw ( TEXT( "QueryPerformanceFrequency() failed" ) );

	return proc_freq.QuadPart;
}

void Time::frameBegin( void )
{
	DWORD_PTR oldmask = ::SetThreadAffinityMask( ::GetCurrentThread(), 0 );

	::QueryPerformanceCounter( &begin );

	::SetThreadAffinityMask( ::GetCurrentThread(), oldmask );
}

double Time::frameEnd( void )
{
	DWORD_PTR oldmask = ::SetThreadAffinityMask( ::GetCurrentThread(), 0 );

	::QueryPerformanceCounter( &end );

	::SetThreadAffinityMask( ::GetCurrentThread(), oldmask );

	return ( deltaTime = ( (end.QuadPart - begin.QuadPart) / freq) );
}

double Time::deltaTimeActual( void )
{
	return clamp( deltaTime, 0, MAXDT );	//Clamp deltaTime to prevent "Spiral of Death"
}

double Time::deltaTimeS( void )
{
	return clamp( deltaTime * timeScaleGlobal, 0, MAXDT * timeScaleGlobal );
}

double Time::fixedStepActual( void )
{
	return clamp( fixedStep, 0, fixedStep );
}

double Time::fixedStepS( void )
{
	return clamp( fixedStep * timeScaleGlobal, 0, fixedStep * timeScaleGlobal );	//Should physics timestep also be affected by timeScale?
}

void Time::fixedStepS( double t )
{
	t = clamp( t, 0, t );
	fixedStep = t;
}

double Time::timeScale( void )
{
	return clamp( timeScaleGlobal, 0.0f, timeScaleGlobal );
}

void Time::timeScale( double t )
{
	t = clamp( t, 0, t );
	timeScaleGlobal = t;
}

double Time::Acc( void )
{
	return physicsAccumulator;
}

void Time::AddToAcc( double t )
{
	physicsAccumulator += t;
}

void Time::SubFromAcc( double t )
{
	physicsAccumulator -= t;
}