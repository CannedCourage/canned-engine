#include "Graphics/Font.h"
#include "System/System.h"

Font::Font( System &s ) : graphics( s.graphics ), text(), font( NULL ), position(), alignment( DT_TOP | DT_LEFT ), colour( 0xffffffff ), description()
{
}

Font::~Font( void )
{
	Destroy();
}

void Font::Create( void )
{
	Destroy();

	D3DXCreateFontIndirectA( graphics.Device(), &description, &font );
}

void Font::Create( const D3DXFONT_DESCA& desc )
{
	description = desc;
	
	Create();
}

void Font::Destroy( void )
{
	if( font != NULL )
	{
		font->Release();
		font = NULL;
	}
}

void Font::SetText( const char* string )
{
	text.assign( string );
}

void Font::SetPosition( const long& top, const long& left, const long& bottom, const long& right )
{
	position.top = top;
	position.left = left;
	position.bottom = bottom;
	position.right = right;
}

void Font::SetPosition( const RECT& r )
{
	position = r;
}

void Font::SetAlignment( const DWORD& a )
{
	alignment = a;
}

void Font::SetColour( const D3DCOLOR c )
{
	colour = c;
}

void Font::SetHeight( const int& height )
{
	description.Height = height;
}

void Font::SetWidth( const int& width )
{
	description.Width = width;
}

void Font::SetWeight( const int& weight )
{
	description.Weight = weight;
}

void Font::SetMipLevel( const int& mip )
{
	description.MipLevels = mip;
}

void Font::SetItalic( const bool& i )
{
	description.Italic = i;
}

void Font::SetCharSet( const BYTE& set )
{
	description.CharSet = set;
}

void Font::SetOutputPrecision( const BYTE& precision )
{
	description.OutputPrecision = precision;
}

void Font::SetQuality( const BYTE& quality )
{
	description.Quality = quality;
}

void Font::SetPitchAndFamily( const BYTE& pf )
{
	description.PitchAndFamily = pf;
}

void Font::SetFont( const char* name )
{
	strcpy( description.FaceName, name );
}

void Font::Draw( void )
{
	//font->DrawTextA( NULL, text.data(), -1, &position, DT_CENTER | DT_BOTTOM, 0xffffffff );
	font->DrawTextA( NULL, text.data(), -1, &position, alignment, colour );
}

void Font::Draw( const char* string )
{
	font->DrawTextA( NULL, string, -1, &position, alignment, colour );
}

void Font::OnLost( void )
{
	graphics.ErrorCheck( font->OnLostDevice(), TEXT( "Font: On Lost Device Called" ) );
}

void Font::OnRecover( void )
{
	graphics.ErrorCheck( font->OnResetDevice(), TEXT( "Font: On Reset Device Called" ) );
}
