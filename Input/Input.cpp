#include "Input/Input.h"
#include "System/System.h"

#include "Input/XboxController.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"

using std::vector;

Input::Input( System& s ) : log( "Input" ), system( s ) , settings( system.settings )
{
}

Input::~Input( void )
{
}

void Input::Init( void )
{
	//mouse.RegisterForRawInput( system.window );
	//keyboard.RegisterForRawInput( system.window );

	//pad1.CheckForPad();
}

void Input::Update( void )
{
	//pad1.GetPadState();
	
	std::vector<Mouse*>::iterator mouseIt;
	std::vector<Keyboard*>::iterator keyIt;
	std::vector<XboxController*>::iterator padIt;
	
	for( mouseIt = mice.begin(); mouseIt != mice.end(); mouseIt++ )
	{
		(*mouseIt)->Update();
	}

	for( keyIt = keyboards.begin(); keyIt != keyboards.end(); keyIt++ )
	{
		(*keyIt)->Update();
	}

	for( padIt = pads.begin(); padIt != pads.end(); padIt++ )
	{
		(*padIt)->Update();
	}
}

void Input::PostUpdate( void )
{
	std::vector<Mouse*>::iterator mouseIt;
	std::vector<Keyboard*>::iterator keyIt;
	std::vector<XboxController*>::iterator padIt;

	for( mouseIt = mice.begin(); mouseIt != mice.end(); mouseIt++ )
	{
		(*mouseIt)->PostUpdate();
	}

	for( keyIt = keyboards.begin(); keyIt != keyboards.end(); keyIt++ )
	{
		(*keyIt)->PostUpdate();
	}

	/*for( padIt = pads.begin(); padIt != pads.end(); padIt++ )
	{
		(*padIt)->PreUpdate();
	}*/
}

void Input::Register( Mouse* mouse )
{
	mouse->RegisterForRawInput( system.window.getHandle() );
	
	mice.push_back( mouse );
}

void Input::Register( Keyboard* keyboard )
{
	keyboard->RegisterForRawInput( system.window.getHandle() );
	
	keyboards.push_back( keyboard );
}

void Input::Register( XboxController* pad )
{
	pads.push_back( pad );
}

void Input::ReceiveRawInput( RAWINPUT* in )
{
	if( in->header.dwType == RIM_TYPEMOUSE )
	{
		vector<Mouse*>::iterator mouseIt;

		for( mouseIt = mice.begin(); mouseIt < mice.end(); mouseIt++ )
		{
			(*mouseIt)->ReceiveRawInput( in->data.mouse );
		}
	}
	
	if( in->header.dwType == RIM_TYPEKEYBOARD )
	{
		vector<Keyboard*>::iterator keyboardIt;

		for( keyboardIt = keyboards.begin(); keyboardIt < keyboards.end(); keyboardIt++ )
		{
			(*keyboardIt)->ReceiveRawInput( in->data.keyboard );
		}
	}
}