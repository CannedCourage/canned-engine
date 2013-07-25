#ifndef _FONT_H_
#define _FONT_H_

#include "Graphics/Graphics.h"
#include <string>

class System;

class Font
{
private:
protected:

	Graphics& graphics;

	std::string text;

	LPD3DXFONT font;
	RECT position;
	DWORD alignment;
	D3DCOLOR colour;

	D3DXFONT_DESCA description;
public:

	Font( System &s );
	~Font( void );

	void Create( void );
	void Create( const D3DXFONT_DESCA& desc );
	void Destroy( void );

	void SetText( const char* string );
	void SetPosition( const long& top, const long& left, const long& bottom, const long& right );
	void SetPosition( const RECT& r );
	void SetAlignment( const DWORD& a );
	void SetColour( const D3DCOLOR c );

	void SetHeight( const int& height );
	void SetWidth( const int& width );
	void SetWeight( const int& weight );
	void SetMipLevel( const int& mip );
	void SetItalic( const bool& i );
	void SetCharSet( const BYTE& set );
	void SetOutputPrecision( const BYTE& precision );
	void SetQuality( const BYTE& quality );
	void SetPitchAndFamily( const BYTE& pf );
	void SetFont( const char* name );	//Constrain length of string? 32 char limit, inc null terminator

	void Draw( void );
	void Draw( const char* string );

	void OnLost( void );
	void OnRecover( void );
};

#endif //_FONT_H_