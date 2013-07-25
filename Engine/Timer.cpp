#include "Engine/Timer.h"

Timer::Timer( void ) : start( ), stop( ), freq( 0.0f )
{
	freq = getFreq();
}

Timer::~Timer( void )
{
}

double Timer::getFreq( void )
{
	LARGE_INTEGER proc_freq;

	if (!::QueryPerformanceFrequency(&proc_freq))
		throw (TEXT("QueryPerformanceFrequency() failed"));

	return proc_freq.QuadPart;
}

void Timer::startTimer( void )
{
	DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);

	::QueryPerformanceCounter(&start);

	::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
}

double Timer::stopTimer( void )
{
	DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);

	::QueryPerformanceCounter(&stop);

	::SetThreadAffinityMask(::GetCurrentThread(), oldmask);

	return ((stop.QuadPart - start.QuadPart) / freq);
}

double Timer::storedTimeS( void )
{
	return ((stop.QuadPart - start.QuadPart) / freq);
}

double Timer::storedTimeMS( void )
{
	return ( storedTimeS() * 1000.0f );
}

double Timer::storedTimeUS( void )
{
	return ( storedTimeS() * 1000000.0f );
}

double Timer::s( void )
{
	LARGE_INTEGER val;

    QueryPerformanceCounter( &val );

	return (val.QuadPart - start.QuadPart) * freq;
}

double Timer::ms( void )
{
	return ( s() * 1000.0f );
}

double Timer::us( void )
{
	return ( s() * 1000000.0f );
}
