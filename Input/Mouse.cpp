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

long Mouse::GetMouseXRelative( void )
{
	return mouse.lLastX;
}

long Mouse::GetMouseYRelative( void )
{
	return mouse.lLastY;
}

short Mouse::GetWheelDelta( void )
{
	if( ( mouse.usButtonFlags & RI_MOUSE_WHEEL ) == RI_MOUSE_WHEEL )
	{
		return (short)mouse.usButtonData;
	}
	else
	{
		return 0;
	}
}

bool Mouse::IsPressed( int button )
{
	return ( ( mouse.usButtonFlags & button ) == button );
}

bool Mouse::WentDown( int button )
{
	return ( ( mouse.usButtonFlags & button ) == button );
}

bool Mouse::WentUp( int button )
{
	button *= 2; //Up constant is 2x Down constant

	return ( ( mouse.usButtonFlags & button ) == button );
}