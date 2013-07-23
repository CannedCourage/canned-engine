#include "GUI\GUI.h"
#include "System\System.h"

GUI::GUI( System &s, const char* n ) : IScene( s, n )
{
}

GUI::~GUI( void )
{
}

void GUI::AddWidget( Widget* w )
{
	widgets.push_back( w );
}