#include "Input/Mouse.h"
#include "input/Input.h"

Log Mouse::log( "Mouse" );

Mouse::Mouse( void )
{
}

Mouse::Mouse( Input& input )
{
	input.Register( this );
}

void Mouse::RegisterForRawInput( HWND hWnd )
{
	RAWINPUTDEVICE Rid[1];

	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
    Rid[0].dwFlags = NULL; //RIDEV_NOLEGACY - Prevent other input messages being sent?
    Rid[0].hwndTarget = hWnd;

    RegisterRawInputDevices( Rid, 1, sizeof( Rid[0] ) );
}

void Mouse::ReceiveRawInput( const RAWMOUSE& input )
{
	currentMouse = input;

	if( ( currentMouse.usButtonFlags & Button::LEFT ) == Button::LEFT ){ buttonStates[ Button::LEFT ] = true; }
	if( ( currentMouse.usButtonFlags & ( Button::LEFT * 2 ) ) == ( Button::LEFT * 2 ) ){ buttonStates[ Button::LEFT ] = false; }

	if( ( currentMouse.usButtonFlags & Button::RIGHT ) == Button::RIGHT ){ buttonStates[ Button::RIGHT ] = true; }
	if( ( currentMouse.usButtonFlags & ( Button::RIGHT * 2 ) ) == ( Button::RIGHT * 2 ) ){ buttonStates[ Button::RIGHT ] = false; }

	if( ( currentMouse.usButtonFlags & Button::MIDDLE ) == Button::MIDDLE ){ buttonStates[ Button::MIDDLE ] = true; }
	if( ( currentMouse.usButtonFlags & ( Button::MIDDLE * 2 ) ) == ( Button::MIDDLE * 2 ) ){ buttonStates[ Button::MIDDLE ] = false; }

	if( ( currentMouse.usButtonFlags & Button::MOUSE4 ) == Button::MOUSE4 ){ buttonStates[ Button::MOUSE4 ] = true; }
	if( ( currentMouse.usButtonFlags & ( Button::MOUSE4 * 2 ) ) == ( Button::MOUSE4 * 2 ) ){ buttonStates[ Button::MOUSE4 ] = false; }

	if( ( currentMouse.usButtonFlags & Button::MOUSE5 ) == Button::MOUSE5 ){ buttonStates[ Button::MOUSE5 ] = true; }
	if( ( currentMouse.usButtonFlags & ( Button::MOUSE5 * 2 ) ) == ( Button::MOUSE5 * 2 ) ){ buttonStates[ Button::MOUSE5 ] = false; }
}

void Mouse::Update( void )
{
}

void Mouse::PostUpdate( void )
{
	ZeroMemory( &currentMouse, sizeof( RAWMOUSE ) );
}

long Mouse::GetMouseXRelative( void )
{
	return currentMouse.lLastX;
}

long Mouse::GetMouseYRelative( void )
{
	return currentMouse.lLastY;
}

short Mouse::GetWheelDelta( void )
{
	if( ( currentMouse.usButtonFlags & RI_MOUSE_WHEEL ) == RI_MOUSE_WHEEL )
	{
		return (short)currentMouse.usButtonData;
	}
	else
	{
		return 0;
	}
}

bool Mouse::IsPressed( int button )
{
	return ( buttonStates[ button ] );
}

bool Mouse::WentDown( int button )
{
	return ( ( currentMouse.usButtonFlags & button ) == button );
}

bool Mouse::WentUp( int button )
{
	button *= 2; //Up constant is 2x Down constant

	return ( ( currentMouse.usButtonFlags & button ) == button );
}