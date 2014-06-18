#include "Game/Interfaces/MenuOption.h"
#include "System/System.h"

MenuOption::MenuOption( System& sys, const char* _name, GUI& _parent ) : Widget( sys, _name, _parent ), displayText( "" )
{
	D3DXFONT_DESC FontDesc = { 24, 0, 400, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_PITCH, TEXT( "Arial" ) };
	graphics.ErrorCheck( D3DXCreateFontIndirect( graphics.Device(), &FontDesc, &font ), "Create font indirect" );

	FontPosition.top = 0;
	FontPosition.left = 0;
	FontPosition.right = sys.settings.GetInteger( "display/xResolution" );
	FontPosition.bottom = sys.settings.GetInteger( "display/yResolution" );
	
	graphics.ErrorCheck( D3DXCreateSprite( graphics.Device(), &sprite ), "Creating Font Sprite" );
	font->DrawText( NULL, displayText, -1, &FontPosition, DT_CALCRECT, 0xffffffff );
}

MenuOption::~MenuOption( void )
{
}

void MenuOption::SetDisplayText( const char* text )
{
	displayText = text;
}

void MenuOption::SetPosition( int x, int y )
{
	FontPosition.top = y;
	FontPosition.left = x;

	font->DrawText( NULL, displayText, -1, &FontPosition, DT_CALCRECT, 0xffffffff );
}

void MenuOption::NextOption( void )
{
    Blur();
    next->Focus();
}

void MenuOption::PreviousOption( void )
{
    Blur();
    prev->Focus();
}

void MenuOption::SetNext( MenuOption* _next )
{
	next = _next;
}

void MenuOption::SetPrev( MenuOption* _prev )
{
	prev = _prev;
}

void MenuOption::Update( void )
{
	//Check for clicks
}

void MenuOption::Render( void )
{
	font->DrawText( NULL, displayText, -1, &FontPosition, DT_CENTER | DT_TOP, 0xffffffff );
}