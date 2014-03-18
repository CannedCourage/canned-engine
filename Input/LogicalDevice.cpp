#include "Input\LogicalDevice.h"
#include "Input\PhysicalDevice.h"

Controller::Controller( void ) : anyPressed( false ), chordPressed( false )
{
}

void Controller::AddInput( PhysicalDevice* k, MethodPointer f, const int& arg )
{
	devices.push_back( k );
	functions.push_back( f );
	args.push_back( arg );
}

void Controller::EvaluateInputs( void )
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

		if( triggeredInputs = numInputs )
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

bool Controller::IsAnyPressed( void )
{
	return anyPressed;
}

bool Controller::IsChordPressed( void )
{
	return chordPressed;
}