#include "Input/Input.h"
#include "System/System.h"
#include <sstream>

using std::ostringstream;

Input::Input( System& s ) : log( "Input" ), system( s ) , settings( system.settings )
{
	CheckForPads();

	int i = XUSER_MAX_COUNT;

	ostringstream o;

	o << "XUSER_MAX_COUNT = " << i << ", Pads Connected: " << padConnected[0] << padConnected[1] << padConnected[2] << padConnected[3];

	log.Message( o.str().c_str() );
}

Input::~Input( void )
{
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