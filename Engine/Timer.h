#ifndef _TIMER_H_
#define _TIMER_H_

#include <Windows.h>

class Timer
{
private:
protected:

	LARGE_INTEGER start, stop;
	double freq;
public:

	Timer( void );
	~Timer( void );

	double getFreq( void );

	void startTimer( void );

	//Stops timer, stores the time, and returns the elapsed time since start
	double stopTimer( void );

	double storedTimeS( void );
	double storedTimeMS( void );
	double storedTimeUS( void );

	//These return time since start in various units
	double s( void );
	double ms( void );
	double us( void );
};

#endif //_TIMER_H_