#include "GUI/GUI.h"
#include "GUI/Widget.h"

GUI::GUI( System& sys, const char* _name, GUI* _parent ) : name( _name ), parent( _parent )
{
}

GUI::~GUI( void )
{
}