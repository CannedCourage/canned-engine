#include "Input/Mouse.h"
#include "input/Input.h"

Mouse::Mouse( void )
{
}

Mouse::Mouse( Input& input )
{
	input.Register( this );
}

void Mouse::ReceiveMousePosition( double X, double Y )
{
	CurrentPos.X = X;
	CurrentPos.Y = Y;
}

void Mouse::ReceiveMouseInput( int Button, int Action, int Mods )
{
	if( Action == 2 )
	{
		return;
	}

	if( Action == 1 )
	{
		CurrentPresses[Button] = DOWN;
	}
	
	if( Action == 0 )
	{
		CurrentPresses[Button] = UP;
	}
}

void Mouse::ReceiveScrollInput( double X, double Y )
{
	CurrentScroll.X = X;
	CurrentScroll.Y = Y;
}

void Mouse::PreUpdate( void )
{
}

void Mouse::Update( void )
{
}

void Mouse::PostUpdate( void )
{
	PreviousPos = CurrentPos;
	PreviousPresses = CurrentPresses;
	CurrentScroll.X = 0;
	CurrentScroll.Y = 0;
}

double Mouse::GetMouseXRelative( void )
{
	return ( CurrentPos.X - PreviousPos.X );
}

double Mouse::GetMouseYRelative( void )
{
	return ( CurrentPos.Y - PreviousPos.Y );
}

double Mouse::GetWheelDelta( void )
{
	return CurrentScroll.Y;
}

bool Mouse::IsPressed( int Button )
{
	return ( CurrentPresses[Button] == DOWN );
}

bool Mouse::WentDown( int Button )
{
	return ( IsPressed( Button ) && ( ( PreviousPresses[Button] == 0 ) || ( PreviousPresses[Button] == UP ) ) );
}

bool Mouse::WentUp( int Button )
{
	return ( !IsPressed( Button ) && ( PreviousPresses[Button] == DOWN ) );
}