#include "Input/Input.h"
#include "System/System.h"
#include <sstream>

using std::ostringstream;

Input::Input( System& s ) : log( "Input" ), system( s ) , settings( system.settings )
{
}

Input::~Input( void )
{
}

void Input::Init( void )
{
	CheckForPads();

	int i = XUSER_MAX_COUNT;

	ostringstream o;

	o << "XUSER_MAX_COUNT = " << i << ", Pads Connected: " << padConnected[0] << padConnected[1] << padConnected[2] << padConnected[3];

	log.Message( o.str().c_str() );

	RegisterMouseForRawInput();
}

void Input::Update( void )
{
	GetPadStates();
}

bool Input::PadConnected( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return padConnected[player];
	}
	else
	{
		return false;
	}
}

short Input::LeftThumbX( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return padState[player].Gamepad.sThumbLX;
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

short Input::LeftThumbY( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return padState[player].Gamepad.sThumbLY;
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

short Input::RightThumbX( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return padState[player].Gamepad.sThumbRX;
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

short Input::RightThumbY( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return padState[player].Gamepad.sThumbRY;
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

BYTE Input::LeftTrigger( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return padState[player].Gamepad.bLeftTrigger;
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

BYTE Input::RightTrigger( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return padState[player].Gamepad.bRightTrigger;
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

bool Input::DPadUp( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return ( ( padState[player].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP ) == XINPUT_GAMEPAD_DPAD_UP );
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

bool Input::DPadDown( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return ( ( padState[player].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN ) == XINPUT_GAMEPAD_DPAD_DOWN );
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

bool Input::DPadLeft( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return ( ( padState[player].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT ) == XINPUT_GAMEPAD_DPAD_LEFT );
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

bool Input::DPadRight( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return ( ( padState[player].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ) == XINPUT_GAMEPAD_DPAD_RIGHT );
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

bool Input::Start( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return ( ( padState[player].Gamepad.wButtons & XINPUT_GAMEPAD_START ) == XINPUT_GAMEPAD_START );
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

bool Input::Back( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return ( ( padState[player].Gamepad.wButtons & XINPUT_GAMEPAD_BACK ) == XINPUT_GAMEPAD_BACK );
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

bool Input::LeftThumbButton( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return ( ( padState[player].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB ) == XINPUT_GAMEPAD_LEFT_THUMB );
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

bool Input::RightThumbButton( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return ( ( padState[player].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ) == XINPUT_GAMEPAD_RIGHT_THUMB );
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

bool Input::LeftBumper( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return ( ( padState[player].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ) == XINPUT_GAMEPAD_LEFT_SHOULDER );
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

bool Input::RightBumper( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return ( ( padState[player].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ) == XINPUT_GAMEPAD_RIGHT_SHOULDER );
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

bool Input::A( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return ( ( padState[player].Gamepad.wButtons & XINPUT_GAMEPAD_A ) == XINPUT_GAMEPAD_A );
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

bool Input::B( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return ( ( padState[player].Gamepad.wButtons & XINPUT_GAMEPAD_B ) == XINPUT_GAMEPAD_B );
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

bool Input::X( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return ( ( padState[player].Gamepad.wButtons & XINPUT_GAMEPAD_X ) == XINPUT_GAMEPAD_X );
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

bool Input::Y( unsigned short player )
{
	if( ( player >= 0 ) && ( player < XUSER_MAX_COUNT ) )
	{
		return ( ( padState[player].Gamepad.wButtons & XINPUT_GAMEPAD_Y ) == XINPUT_GAMEPAD_Y );
	}
	else
	{
		throw( "Attempting to access controller that is not connected." );
	}
}

void Input::CheckForPads( void )
{
	XINPUT_STATE state;

	for ( int i = 0; i < XUSER_MAX_COUNT; i++ )
	{
		ZeroMemory( &state, sizeof( XINPUT_STATE ) );
		padConnected[i] = false;
 
		if ( XInputGetState( i, &state ) == ERROR_SUCCESS )
		{
			padConnected[i] = true;
		}
	}
}

void Input::GetPadStates( void )
{
	for ( int i = 0; i < XUSER_MAX_COUNT; i++ )
	{
		if( padConnected[i] == true )
		{
			ZeroMemory( &padState[i], sizeof( XINPUT_STATE ) );
	 
			if ( XInputGetState( i, &padState[i] ) == ERROR_SUCCESS )
			{
				padConnected[i] = true;
			}
		}
	}
}

void Input::RegisterMouseForRawInput( void )
{
	WindowMS& winMS = static_cast<WindowMS&>(system.window);

	HWND hWnd = winMS.getHandle();

	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    Rid[0].hwndTarget = hWnd;

    RegisterRawInputDevices( Rid, 1, sizeof( Rid[0] ) );
}

void Input::ReceiveRawMouseInput( const RAWMOUSE& input )
{
	mouse = input;
}

int Input::GetMouseXRelative( void )
{
	return mouse.lLastX;
}

int Input::GetMouseYRelative( void )
{
	return mouse.lLastY;
}