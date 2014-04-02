#include "Input/LogicalDevice.h"
#include "Input/PhysicalDevice.h"
#include "Input/Input.h"

LogicalDevice::LogicalDevice( void ) : anyPressed( false ), chordPressed( false )
{
}

LogicalDevice::LogicalDevice( Input& input ) : anyPressed( false ), chordPressed( false )
{
	input.Register( this );
}

void LogicalDevice::AddInput( PhysicalDevice* k, MethodPointer f, const int& arg )
{
	devices.push_back( k );
	functions.push_back( f );
	args.push_back( arg );
}

void LogicalDevice::EvaluateInputs( void )
{
	const int numInputs = devices.size();

	int triggeredInputs = 0;

	for( int i = 0; i < numInputs; i++ )
	{
		//Note brackets around class->method pointer
		if( ( devices[i]->*functions[i] )( args[i] ) )
		{
			triggeredInputs++;
		}
	}

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