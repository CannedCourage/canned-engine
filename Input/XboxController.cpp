#include "XboxController.h"
#include <sstream>

using std::ostringstream;

XboxController::XboxController( int playerID ) : log( "XboxController" ), PID( playerID )
{
	CheckForPad();

	ostringstream o;

	o << "XUSER_MAX_COUNT = " << XUSER_MAX_COUNT << ", Pad " << PID << " Connected: " << padConnected;

	log.Message( o.str().c_str() );
}

void XboxController::CheckForPad( void )
{
	XINPUT_STATE state;

	ZeroMemory( &state, sizeof( XINPUT_STATE ) );
	padConnected = false;

	if( XInputGetState( PID, &state ) == ERROR_SUCCESS )
	{
		padConnected = true;
	}
}

void XboxController::GetPadState( void )
{
	if( padConnected == true )
	{
		ZeroMemory( &padState, sizeof( XINPUT_STATE ) );
 
		if( XInputGetState( PID, &padState ) == ERROR_SUCCESS )
		{
			padConnected = true;
		}
	}
}

void XboxController::Update( void )
{
	CheckForPad();
	GetPadState();
}

bool XboxController::PadConnected( void )
{
	return padConnected;
}

bool XboxController::IsPressed( XboxController::Button button )
{
	return ( ( padState.Gamepad.wButtons & button ) == button );
}

bool XboxController::WentDown( XboxController::Button button )
{
	return true;
}

bool XboxController::WentUp( XboxController::Button button )
{
	return true;
}

short XboxController::LeftThumbX( void )
{
	return padState.Gamepad.sThumbLX;
}

short XboxController::LeftThumbY( void )
{
	return padState.Gamepad.sThumbLY;
}

short XboxController::RightThumbX( void )
{
	return padState.Gamepad.sThumbRX;
}

short XboxController::RightThumbY( void )
{
	return padState.Gamepad.sThumbRY;
}

BYTE XboxController::LeftTrigger( void )
{
	return padState.Gamepad.bLeftTrigger;
}

BYTE XboxController::RightTrigger( void )
{
	return padState.Gamepad.bRightTrigger;
}