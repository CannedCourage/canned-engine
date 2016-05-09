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

	EngineTime StartOfFrame{};
	EngineTime EndOfFrame{};

	EngineDuration FrameDeltaTime{ 0 };
	EngineDuration PhysicsAcc{ 0 };
	EngineDuration FixedPhysicsStep{ 100 };

	double TimeScaleFactor = 1.0f;

	const EngineDuration MAXDELTATIME{ 250 };
public:

	Time( void ) = default;
	~Time( void ) = default;

	void FrameBegin( void );
	
	EngineDuration FrameEnd( void ); //Stops timer, stores the time, and returns the elapsed time as deltaTime

	//Get the dt in milliseconds
	EngineDuration DeltaTimeActual( void );
	
	//Get the dt in seconds (ms), this is multiplied by TimeScaleFactor before being returned
	EngineDuration DeltaTimeScaled( void );

	//Get the fixed physics step in seconds (ms)
	EngineDuration FixedStepActual( void );
	
	//Get the fixed physics step in seconds (ms), this is multiplied by TimeScaleFactor before being returned
	EngineDuration FixedStepScaled( void );

	//Set the fixed physics step (shouldn't be lower than 1/100?)
	void SetFixedStep( int Milliseconds );

	//Set the TimeScaleFactor
	void SetTimeScale( double Multiplier );

	//Get the current TimeScaleFactor
	double GetTimeScale( void );

	EngineDuration GetPhysicsAccumulator( void );

	void AddToPhysicsAccumulator( EngineDuration TimeToAdd );

	void SubtractFromPhysicsAccumulator( EngineDuration TimeToSubtract );
};

#endif //_TIMER_H_