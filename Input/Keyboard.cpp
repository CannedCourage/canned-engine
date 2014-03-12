#include "Keyboard.h"
#include "Window\WindowMS.h"

Keyboard::Keyboard( void ) : log( "Keyboard" )
{
}

void Keyboard::RegisterForRawInput( iWindow& win )
{
	WindowMS& winMS = static_cast<WindowMS&>(win);

	HWND hWnd = winMS.getHandle();

	RAWINPUTDEVICE Rid[1];

	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    Rid[0].usUsage = HID_USAGE_GENERIC_KEYBOARD;
    Rid[0].dwFlags = NULL; //RIDEV_INPUTSINK
    Rid[0].hwndTarget = hWnd;

    RegisterRawInputDevices( Rid, 1, sizeof( Rid[0] ) );
}

void Keyboard::ReceiveRawKeyboardInput( const RAWKEYBOARD& input )
{
	keyboard = input;
}

bool Keyboard::IsPressed( Keyboard::Key key )
{
	return true;
}

bool Keyboard::WentDown( Keyboard::Key key )
{
	return true;
}

bool Keyboard::WentUp( Keyboard::Key key )
{
	return true;
}