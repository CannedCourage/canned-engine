#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>

typedef std::chrono::milliseconds EngineDuration;
typedef std::chrono::high_resolution_clock EngineClock;
typedef EngineClock::time_point EngineTime;

class Time
{
private:
protected:

	EngineTime StartOfFrame, EndOfFrame;

	EngineDuration FrameDeltaTime, FixedPhysicsStep, PhysicsAcc;
	double TimeScaleFactor;

	const EngineDuration MAXDELTATIME;
public:

	Time( void );
	~Time( void );

	void FrameBegin( void );
	EngineDuration FrameEnd( void ); //Stops timer, stores the time, and returns the elapsed time as deltaTime

	//Get the dt in seconds
	EngineDuration DeltaTimeActual( void );
	//Get the dt in seconds (s), this is multiplied by timeScale before being returned
	EngineDuration DeltaTime( void );

	//Get the fixed physics step in seconds (s)
	EngineDuration FixedStepActual( void );
	//Get the fixed physics step in seconds (s), this is multiplied by timeScale before being returned
	EngineDuration FixedStep( void );

	//Set the fixed physics step (shouldn't be lower than 1/100?)
	void FixedStep( int milliseconds );

	//Set the timeScale
	void TimeScale( double multiplier );

	//Get the current timeScale
	double TimeScale( void );

	EngineDuration PhysicsAccumulator( void );

	void AddToPhysicsAccumulator( EngineDuration time );

	void SubtractFromPhysicsAccumulator( EngineDuration time );
};

#endif //_TIMER_H_