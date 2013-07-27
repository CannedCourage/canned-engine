#ifndef _TIMER_H_
#define _TIMER_H_

#include <Windows.h>

class Time
{
private:
protected:

	LARGE_INTEGER begin, end;
	double freq, deltaTime, fixedStep, timeScaleGlobal;
	double physicsAccumulator;

	double getFreq( void );

	const float MAXDT;
public:

	Time( void );
	~Time( void );

	void frameBegin( void );
	double frameEnd( void );	//Stops timer, stores the time, and stores the elapsed time as deltaTime

	//Get the dt in seconds
	double deltaTimeActual( void );
	//Get the dt in seconds (s), this is multiplied by timeScale before being returned
	double deltaTimeS( void );

	//Get the fixed physics step in seconds (s)
	double fixedStepActual( void );
	//Get the fixed physics step in seconds (s), this is multiplied by timeScale before being returned
	double fixedStepS( void );

	//Set the fixed physics step (shouldn't be lower than 1/10?)
	void fixedStepS( double t );

	//Get the current timeScale
	double timeScale( void );

	//Set the timeScale
	void timeScale( double t );

	double Acc( void );
	void AddToAcc( double t );
	void SubFromAcc( double t );
};

#endif //_TIMER_H_