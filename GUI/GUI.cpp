#include "GUI\GUI.h"

GUI::GUI( const char* n ) : IScene( n )
{
}

GUI::~GUI( void )
{
}

void GUI::AddWidget( Widget* w )
{
	widgets.push_back( w );
}