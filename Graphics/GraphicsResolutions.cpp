#include "Graphics/Graphics.h"
#include "Window/WindowMS.h"

#include <iostream>
#include <sstream>

using std::string;
using std::ostringstream;

using std::vector;

void Graphics::SelectAspect( const int& index )
{
	switch( index )
	{
		case 43: aspect = (ASPECT)index;
				break;
		case 169: aspect = (ASPECT)index;
				break;
		case 1610: aspect = (ASPECT)index;
				break;
		default: aspect = (ASPECT)43;
				break;
	}
}

void Graphics::GetModesFromInterface( void )
{
	modeCount = mInterface->GetAdapterModeCount( adapter, backbufferFormat );

	if( modes != NULL )
	{
		delete[] modes;
		mode = NULL;
	}
	
	modes = new D3DDISPLAYMODE[modeCount];

	int j = 1;

	char temp[256];

	TRACE( "Checking Modes" );

	for( UINT i = 0; i < modeCount; i++ )
	{
		ErrorCheck( mInterface->EnumAdapterModes( adapter, backbufferFormat, i, &modes[i] ), "Enumerating Display Modes Failed" );

		ostringstream o;

		o << modes[i].Width << " x " << modes[i].Height << ", " << modes[i].RefreshRate << "hz, " << "Index: " << i;

		string temp = o.str();

		TRACE( temp.c_str() );
	}
}

void Graphics::SelectResolution( const int& index )
{
	refresh = modes[index].RefreshRate;
	SetResolution( modes[index].Width, modes[index].Height );
}

void Graphics::SetResolution( const int& width, const int& height )
{
	xResolution = width;
	yResolution = height;

	SetClientSize( width, height );

	Refresh();
}

const std::vector<dMode>& Graphics::GetDisplayModes( void )
{
	for( int i = 0; i < modeCount; i++ )
	{
		dMode d( modes[i].Width, modes[i].Height, modes[i].RefreshRate, i );

		dModes.push_back( d );
	}
	
	return dModes;
}