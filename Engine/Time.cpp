#include "Engine/Time.h"
#include "Maths/Clamp.h"

Time::Time( void ) : StartOfFrame(), EndOfFrame(), FrameDeltaTime( 0 ), FixedPhysicsStep( 10 ),
						PhysicsAcc( 0 ), TimeScaleFactor( 1.0f ), MAXDELTATIME( 250 )
{
}

Time::~Time( void )
{
}

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

EngineDuration Time::DeltaTime( void )
{
	return std::chrono::duration_cast<EngineDuration>( DeltaTimeActual() * TimeScaleFactor );
}

EngineDuration Time::FixedStepActual( void )
{
	return FixedPhysicsStep;
}

EngineDuration Time::FixedStep( void )
{
	return std::chrono::duration_cast<EngineDuration>( FixedPhysicsStep * TimeScaleFactor ); //Should physics timestep also be affected by timeScale?
}

void Time::FixedStep( int milliseconds )
{
	if( milliseconds > 0 )
	{
		FixedPhysicsStep = EngineDuration( milliseconds );
	}
}

double Time::TimeScale( void )
{
	return TimeScaleFactor;
}

void Time::TimeScale( double multiplier )
{
	if( multiplier > 0 )
	{
		TimeScaleFactor = multiplier;
	}
}

EngineDuration Time::PhysicsAccumulator( void )
{
	return PhysicsAcc;
}

void Time::AddToPhysicsAccumulator( EngineDuration time )
{
	PhysicsAcc += time;
}

void Time::SubtractFromPhysicsAccumulator( EngineDuration time )
{
	PhysicsAcc -= time;
}