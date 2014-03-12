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

	if ( XInputGetState( PID, &state ) == ERROR_SUCCESS )
	{
		padConnected = true;
	}
}

void XboxController::GetPadState( void )
{
	if( padConnected == true )
	{
		ZeroMemory( &padState, sizeof( XINPUT_STATE ) );
 
		if ( XInputGetState( PID, &padState ) == ERROR_SUCCESS )
		{
			padConnected = true;
		}
	}
}

bool XboxController::PadConnected( void )
{
	return padConnected;
}

bool XboxController::IsPressed( XboxController::Button button )
{
	return true;
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

bool XboxController::DPadUp( void )
{
	return ( ( padState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP ) == XINPUT_GAMEPAD_DPAD_UP );
}

bool XboxController::DPadDown( void )
{
	return ( ( padState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN ) == XINPUT_GAMEPAD_DPAD_DOWN );
}

bool XboxController::DPadLeft( void )
{
	return ( ( padState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT ) == XINPUT_GAMEPAD_DPAD_LEFT );
}

bool XboxController::DPadRight( void )
{
	return ( ( padState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ) == XINPUT_GAMEPAD_DPAD_RIGHT );
}

bool XboxController::Start( void )
{
	return ( ( padState.Gamepad.wButtons & XINPUT_GAMEPAD_START ) == XINPUT_GAMEPAD_START );
}

bool XboxController::Back( void )
{
	return ( ( padState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK ) == XINPUT_GAMEPAD_BACK );
}

bool XboxController::LeftThumbButton( void )
{
	return ( ( padState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB ) == XINPUT_GAMEPAD_LEFT_THUMB );
}

bool XboxController::RightThumbButton( void )
{
	return ( ( padState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ) == XINPUT_GAMEPAD_RIGHT_THUMB );
}

bool XboxController::LeftBumper( void )
{
	return ( ( padState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ) == XINPUT_GAMEPAD_LEFT_SHOULDER );
}

bool XboxController::RightBumper( void )
{
	return ( ( padState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ) == XINPUT_GAMEPAD_RIGHT_SHOULDER );
}

bool XboxController::AButton( void )
{
	return ( ( padState.Gamepad.wButtons & XINPUT_GAMEPAD_A ) == XINPUT_GAMEPAD_A );
}

bool XboxController::BButton( void )
{
	return ( ( padState.Gamepad.wButtons & XINPUT_GAMEPAD_B ) == XINPUT_GAMEPAD_B );
}

bool XboxController::XButton( void )
{
	return ( ( padState.Gamepad.wButtons & XINPUT_GAMEPAD_X ) == XINPUT_GAMEPAD_X );
}

bool XboxController::YButton( void )
{
	return ( ( padState.Gamepad.wButtons & XINPUT_GAMEPAD_Y ) == XINPUT_GAMEPAD_Y );
}