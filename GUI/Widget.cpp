#include "GUI/Widget.h"

#include "System/System.h"

Widget::Widget( System& sys, const char* _name, GUI& _parent ) : graphics( sys.graphics ), name( _name ), parent( _parent )
{
}

Widget::~Widget( void )
{
}

bool Widget::hasFocus( void )
{
	return focused;
}

void Widget::Focus( void )
{
	focused = true;
}

void Widget::Blur( void )
{
	focused = false;
}