#include "XboxController.h"
#include <sstream>
#include "Maths/Clamp.h"
#include <cmath>

using std::ostringstream;

Log XboxController::log( "XboxController" );

XboxController::XboxController( int playerID, float _deadLX, float _deadLY, float _deadRX, float _deadRY ) : PID( playerID ), deadLX( _deadLX ), deadLY( _deadLY ), deadRX( _deadRX ), deadRY( _deadRY )
{
	ZeroMemory( &previousState, sizeof( XINPUT_STATE ) );

	ostringstream o;

	o << "XUSER_MAX_COUNT = " << XUSER_MAX_COUNT << ", Pad " << PID << " Connected: " << padConnected;

	log.Message( o.str().c_str() );
}

void XboxController::GetPadState( void )
{
	padConnected = false;

	ZeroMemory( &currentState, sizeof( XINPUT_STATE ) );

	if( XInputGetState( PID, &currentState ) == ERROR_SUCCESS )
	{
		padConnected = true;
	}
}

void XboxController::Update( void )
{
	previousState = currentState;

	GetPadState();

	normLX = NormLeftThumbX();
	normLY = NormLeftThumbY();
	normRX = NormRightThumbX();
	normRY = NormRightThumbY();
}

void XboxController::Vibrate( int left, int right )
{
	XINPUT_VIBRATION vibration;
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

    vibration.wLeftMotorSpeed = left;
    vibration.wRightMotorSpeed = right;

    // Vibrate the controller
    XInputSetState( PID, &vibration );
}

void XboxController::SetDeadzone( float _deadLX, float _deadLY, float _deadRX, float _deadRY )
{
	deadLX = _deadLX;
	deadLY = _deadLY;
	deadRX = _deadRX;
	deadRY = _deadRY;
}

bool XboxController::PadConnected( void )
{
	return padConnected;
}

bool XboxController::IsPressed( int button )
{
	if( PadConnected() )
	{
		return ( ( currentState.Gamepad.wButtons & button ) == button );
	}
	else
	{
		false;
	}
}

bool XboxController::WentDown( int button )
{
	bool previousDown = ( ( previousState.Gamepad.wButtons & button ) == button );
	bool currentDown = IsPressed( button );

	return ( ( previousDown = false ) && ( currentDown = true ) );
}

bool XboxController::WentUp( int button )
{
	bool previousDown = ( ( previousState.Gamepad.wButtons & button ) == button );
	bool currentDown = IsPressed( button );

	return ( ( previousDown = true ) && ( currentDown = false ) );
}

float XboxController::NormLeftThumbX( void )
{
	return clamp( ( (float)currentState.Gamepad.sThumbLX / 32767 ), -1.0f, 1.0f );
}

float XboxController::NormLeftThumbY( void )
{
	return clamp( ( (float)currentState.Gamepad.sThumbLY / 32767 ), -1.0f, 1.0f );
}

float XboxController::NormRightThumbX( void )
{
	return clamp( ( (float)currentState.Gamepad.sThumbRX / 32767 ), -1.0f, 1.0f );
}

float XboxController::NormRightThumbY( void )
{
	return clamp( ( (float)currentState.Gamepad.sThumbRY / 32767 ), -1.0f, 1.0f );
}

float XboxController::LeftThumbX( void )
{
	float result = 0.0f;

	if( abs( normLX ) < deadLX )
	{
		//Within deadzone
		result = 0.0f;
	}
	else
	{
		//Treat non-dead area as range from 0 - 1 
		//Adjust movement as if deadzone is beginning of range//Restore sign
		result = ( abs( normLX ) - deadLX ) * ( normLX / abs( normLX ) );

		//Normalise for non-dead range (i.e. adjusted movement / non-dead range)
		result /= ( 1 - deadLX );
	}

	return result;
}

float XboxController::LeftThumbY( void )
{
	float result = 0.0f;

	if( abs( normLY ) < deadLY )
	{
		result = 0.0f;
	}
	else
	{
		result = ( abs( normLY ) - deadLY ) * ( normLY / abs( normLY ) );

		result /= ( 1 - deadLY );
	}

	return result;
}

float XboxController::RightThumbX( void )
{
	float result = 0.0f;

	if( abs( normRX ) < deadRX )
	{
		result = 0.0f;
	}
	else
	{
		result = ( abs( normRX ) - deadRX ) * ( normRX / abs( normRX ) );

		result /= ( 1 - deadRX );
	}

	return result;
}

float XboxController::RightThumbY( void )
{
	float result = 0.0f;

	if( abs( normRY ) < deadRY )
	{
		result = 0.0f;
	}
	else
	{
		result = ( abs( normRY ) - deadRY ) * ( normRY / abs( normRY ) );

		result /= ( 1 - deadRY );
	}

	return result;
}

float XboxController::LeftTrigger( void )
{
	return clamp( ( (float)currentState.Gamepad.bLeftTrigger / 255 ), 0.0f, 1.0f );
}

float XboxController::RightTrigger( void )
{
	return clamp( ( (float)currentState.Gamepad.bRightTrigger / 255 ), 0.0f, 1.0f );
}