#include "Input/Input.h"
#include "System/System.h"

Input::Input( System& s ) : log( "Input" ), system( s ) , settings( system.settings )
{
}

Input::~Input( void )
{
}

void Input::Init( void )
{
	mouse.RegisterForRawInput( system.window );
	keyboard.RegisterForRawInput( system.window );

	pad1.CheckForPad();
	pad2.CheckForPad();
	pad3.CheckForPad();
	pad4.CheckForPad();
}

void Input::Update( void )
{
	pad1.GetPadState();
	pad2.GetPadState();
	pad3.GetPadState();
	pad4.GetPadState();
}

XboxController& Input::Controller( int number )
{
	switch( number )
	{
		case 0: return pad1; break;
		case 1: return pad2; break;
		case 2: return pad3; break;
		case 3: return pad4; break;
		default: return pad1; break;
	}
}

Mouse& Input::Mouse( void )
{
	return mouse;
}

Keyboard& Input::Keyboard( void )
{
	return keyboard;
}