#include "Input/Keyboard.h"
#include "Input/Input.h"

using std::map;

Keyboard::Keyboard( void )
{
}

Keyboard::Keyboard( Input& input )
{
	//TODO: Move this code outside the class
	input.Register( this );
}

void Keyboard::ReceiveKeyboardInput( int Key, int Scancode, int Action, int Mods )
{
	if( Action == 2 )
	{
		return;
	}

	if( Action == 1 )
	{
		currentPresses[Key] = DOWN;
	}
	
	if( Action == 0 )
	{
		currentPresses[Key] = UP;
	}
}

void Keyboard::PreUpdate( void )
{
}

void Keyboard::Update( void )
{
}

void Keyboard::PostUpdate( void )
{
	previousPresses = currentPresses;
}

bool Keyboard::IsPressed( int Key )
{
	//Is the key down now?
	return ( currentPresses[Key] == DOWN );
}

bool Keyboard::WentDown( int Key )
{
	//Is the key down now, when it was up or unknown previously?
	return ( IsPressed( Key ) && ( ( previousPresses[Key] == 0 ) || ( previousPresses[Key] == UP ) ) );
}

bool Keyboard::WentUp( int Key )
{
	//Is the key up now, when it was down previously?
	return ( !IsPressed( Key ) && ( previousPresses[Key] == DOWN ) );
}