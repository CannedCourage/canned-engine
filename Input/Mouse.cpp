#include "Input\Mouse.h"
#include "Window\WindowMS.h"

Mouse::Mouse( void ) : log( "Mouse" )
{
}

void Mouse::RegisterForRawInput( iWindow& win )
{
	WindowMS& winMS = static_cast<WindowMS&>(win);

	HWND hWnd = winMS.getHandle();

	RAWINPUTDEVICE Rid[1];

	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
    Rid[0].dwFlags = NULL; //RIDEV_NOLEGACY - Prevent other input messages being sent?
    Rid[0].hwndTarget = hWnd;

    RegisterRawInputDevices( Rid, 1, sizeof( Rid[0] ) );
}

void Mouse::ReceiveRawMouseInput( const RAWMOUSE& input )
{
	mouse = input;
}

int Mouse::GetMouseXRelative( void )
{
	return mouse.lLastX;
}

int Mouse::GetMouseYRelative( void )
{
	return mouse.lLastY;
}

bool Mouse::IsPressed( Mouse::Button button )
{
	return true;
}

bool Mouse::WentDown( Mouse::Button button )
{
	return true;
}

bool Mouse::WentUp( Mouse::Button button )
{
	return true;
}