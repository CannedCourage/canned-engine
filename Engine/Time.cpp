#include "Engine/Time.h"

void Time::FrameBegin( void )
{
	StartOfFrame = EngineClock::now();
}

EngineDuration Time::FrameEnd( void )
{
	EndOfFrame = EngineClock::now();

	FrameDeltaTime = std::chrono::duration_cast<EngineDuration>( EndOfFrame - StartOfFrame );

	return FrameDeltaTime;
}

EngineDuration Time::DeltaTimeActual( void )
{
	//Clamp deltaTime to prevent "Spiral of Death"

	if( FrameDeltaTime > MAXDELTATIME )
	{
		return MAXDELTATIME;
	}

	if( FrameDeltaTime < EngineDuration::zero() )
	{
		return EngineDuration::zero();
	}

	return FrameDeltaTime;
}

EngineDuration Time::DeltaTimeScaled( void )
{
	return std::chrono::duration_cast<EngineDuration>( DeltaTimeActual() * TimeScaleFactor );
}

EngineDuration Time::FixedStepActual( void )
{
	return FixedPhysicsStep;
}

EngineDuration Time::FixedStepScaled( void )
{
	return std::chrono::duration_cast<EngineDuration>( FixedPhysicsStep * TimeScaleFactor ); //Should physics timestep also be affected by timeScale?
}

void Time::SetFixedStep( int Milliseconds )
{
	if( Milliseconds > 0 )
	{
		FixedPhysicsStep = EngineDuration( Milliseconds );
	}
}

double Time::GetTimeScale( void )
{
	return TimeScaleFactor;
}

void Time::SetTimeScale( double Multiplier )
{
	if( Multiplier > 0 )
	{
		TimeScaleFactor = Multiplier;
	}
}

EngineDuration Time::GetPhysicsAccumulator( void )
{
	return PhysicsAcc;
}

void Time::AddToPhysicsAccumulator( EngineDuration TimeToAdd )
{
	PhysicsAcc += TimeToAdd;
}

void Time::SubtractFromPhysicsAccumulator( EngineDuration TimeToSubtract )
{
	PhysicsAcc -= TimeToSubtract;
}