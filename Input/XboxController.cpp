#include "XboxController.h"
#include <sstream>
#include "Maths/Clamp.h"
#include <cmath>
#include "Input/Input.h"

using std::ostringstream;

Log XboxController::log( "XboxController" );

XboxController::XboxController( int playerID, float _deadLX, float _deadLY, float _deadRX, float _deadRY ) : PID( playerID ), deadLX( _deadLX ), deadLY( _deadLY ), deadRX( _deadRX ), deadRY( _deadRY )
{
	ZeroMemory( &previousState, sizeof( XINPUT_STATE ) );

	ostringstream o;

	o << "XUSER_MAX_COUNT = " << XUSER_MAX_COUNT << ", Pad " << PID << " Connected: " << padConnected;

	TRACE( o.str().c_str() );
}

XboxController::XboxController( Input& input, int playerID, float _deadLX, float _deadLY, float _deadRX, float _deadRY ) : PID( playerID ), deadLX( _deadLX ), deadLY( _deadLY ), deadRX( _deadRX ), deadRY( _deadRY )
{
	ZeroMemory( &previousState, sizeof( XINPUT_STATE ) );

	ostringstream o;

	o << "XUSER_MAX_COUNT = " << XUSER_MAX_COUNT << ", Pad " << PID << " Connected: " << padConnected;

	TRACE( o.str().c_str() );

	input.Register( this );
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

void XboxController::PreUpdate( void )
{
}

void XboxController::Update( void )
{
	previousState = currentState;

	GetPadState();

	trigL = NormaliseTrigger( currentState.Gamepad.bLeftTrigger );
	trigR = NormaliseTrigger( currentState.Gamepad.bRightTrigger );

	float normLX, normLY, normRX, normRY;

	normLX = NormaliseStick( currentState.Gamepad.sThumbLX );
	normLY = NormaliseStick( currentState.Gamepad.sThumbLY );
	normRX = NormaliseStick( currentState.Gamepad.sThumbRX );
	normRY = NormaliseStick( currentState.Gamepad.sThumbRY );

	adjLX = AdjustNormOutput( normLX, deadLX );
	adjLY = AdjustNormOutput( normLY, deadLY );
	adjRX = AdjustNormOutput( normRX, deadRX );
	adjRY = AdjustNormOutput( normRY, deadRY );
}

void XboxController::PostUpdate( void )
{
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
	deadLX = clamp( _deadLX, 0.0f, 1.0f );
	deadLY = clamp( _deadLY, 0.0f, 1.0f );
	deadRX = clamp( _deadRX, 0.0f, 1.0f );
	deadRY = clamp( _deadRY, 0.0f, 1.0f );
}

bool XboxController::PadConnected( void )
{
	return padConnected;
}

bool XboxController::IsPressed( int button )
{
	if( !PadConnected() ){ return false; }

	return ( ( currentState.Gamepad.wButtons & button ) == button );
}

bool XboxController::WentDown( int button )
{
	if( !PadConnected() ){ return false; }

	bool previousDown = ( ( previousState.Gamepad.wButtons & button ) == button );
	bool currentDown = IsPressed( button );

	return ( ( previousDown == false ) && ( currentDown == true ) );
}

bool XboxController::WentUp( int button )
{
	if( !PadConnected() ){ return false; }
	
	bool previousDown = ( ( previousState.Gamepad.wButtons & button ) == button );
	bool currentDown = IsPressed( button );

	return ( ( previousDown == true ) && ( currentDown == false ) );
}

float XboxController::NormaliseStick( int stickOutput )
{
	return clamp( ( (float)stickOutput / 32767 ), -1.0f, 1.0f );
}

float XboxController::AdjustNormOutput( float normalisedOutput, float deadzone )
{
	float result = 0.0f;

	if( abs( normalisedOutput ) < deadzone )
	{
		//Within deadzone
		result = 0.0f;
	}
	else
	{
		//Treat non-dead area as range from 0 - 1 
		//Adjust movement as if deadzone is beginning of range          //Restore sign
		result = ( abs( normalisedOutput ) - deadzone ) * ( normalisedOutput / abs( normalisedOutput ) );

		//Normalise for non-dead range (i.e. adjusted movement / non-dead range)
		result /= ( 1 - deadzone );
	}

	return result;
}

float XboxController::NormaliseTrigger( int trigOutput )
{
	return clamp( ( (float)trigOutput / 255 ), 0.0f, 1.0f );
}

float XboxController::LeftThumbX( void )
{
	return adjLX;
}

float XboxController::LeftThumbY( void )
{
	return adjLY;
}

float XboxController::RightThumbX( void )
{
	return adjRX;
}

float XboxController::RightThumbY( void )
{
	return adjRY;
}

float XboxController::LeftTrigger( void )
{
	return trigL;
}

float XboxController::RightTrigger( void )
{
	return trigR;
}