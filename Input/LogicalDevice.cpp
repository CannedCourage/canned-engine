#include "Input/LogicalDevice.h"
#include "Input/PhysicalDevice.h"
#include "Input/Input.h"

#include <algorithm>

LogicalDevice::LogicalDevice( void ) : anyPressed( false ), chordPressed( false )
{
}

LogicalDevice::LogicalDevice( Input& input ) : anyPressed( false ), chordPressed( false )
{
	input.Register( this );
}

void LogicalDevice::AddInput( InputFunction func )
{
	inputs.push_back( func );
}

void LogicalDevice::EvaluateInputs( void )
{
	const int numInputs = inputs.size();

	int triggeredInputs = 0;

	std::for_each( inputs.begin(), inputs.end(), [&triggeredInputs]( InputFunction func )
	{
		if( func() )
		{
			triggeredInputs++;
		}
	});

	if( triggeredInputs > 0 )
	{
		anyPressed = true;

		if( triggeredInputs == numInputs )
		{
			chordPressed = true;
		}
		else
		{
			chordPressed = false;
		}
	}
	else
	{
		anyPressed = false;
		chordPressed = false;
	}
}

bool LogicalDevice::IsAnyPressed( void )
{
	return anyPressed;
}

bool LogicalDevice::IsChordPressed( void )
{
	return chordPressed;
}