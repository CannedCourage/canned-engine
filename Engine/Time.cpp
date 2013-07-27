#include "Engine/Time.h"

Time::Time( void ) : begin( ), end( ), freq( 0.0f ), deltaTime( 0.0f ), fixedStep( 0.0f ), timeScale( 1.0f )
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

double Time::deltaTimeS( void )
{
	return ( ( end.QuadPart - begin.QuadPart ) / freq );
}

double Time::deltaTimeMS( void )
{
	return ( deltaTimeS() * 1000.0f );
}

double Time::deltaTimeUS( void )
{
	return ( deltaTimeS() * 1000000.0f );
}
