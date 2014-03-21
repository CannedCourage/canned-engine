#include "Input\Mouse.h"

Log Mouse::log( "Mouse" );

Mouse::Mouse( void )
{
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
	mouse = input;
}

void Mouse::Update( void )
{
}

int Mouse::GetMouseXRelative( void )
{
	return mouse.lLastX;
}

int Mouse::GetMouseYRelative( void )
{
	return mouse.lLastY;
}

bool Mouse::IsPressed( int button )
{
	return true;
}

bool Mouse::WentDown( int button )
{
	return true;
}

bool Mouse::WentUp( int button )
{
	return true;
}