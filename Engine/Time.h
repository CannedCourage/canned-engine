#ifndef _TIMER_H_
#define _TIMER_H_

#include <Windows.h>

class Time
{
private:
protected:

	LARGE_INTEGER begin, end;
	double freq, deltaTime, fixedStep, timeScale;

	double getFreq( void );
public:

	Time( void );
	~Time( void );

	void frameBegin( void );
	double frameEnd( void );	//Stops timer, stores the time, and stores the elapsed time as deltaTime

	double deltaTimeS( void );
	double deltaTimeMS( void );
	double deltaTimeUS( void );
};

#endif //_TIMER_H_