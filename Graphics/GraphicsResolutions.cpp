#include "Graphics\Graphics.h"

void Graphics::SelectAspect( int index )
{
	switch( index )
	{
		case 1:
			SelectAspect43();
			break;
		case 2:
			SelectAspect169();
			break;
		case 3:
			SelectAspect1610();
			break;
		default:
			SelectAspect43();
			break;
	}

	GetMode();
}

void Graphics::GetMode( void )
{
	modeCount = mInterface->GetAdapterModeCount( adapter, backbufferFormat );

	if( modes != NULL )
	{
		delete modes;
	}
	
	window.ClearDisplayOptions();

	modes = new D3DDISPLAYMODE[modeCount];

	int j = 1;

	char temp[256];

	for( UINT i = 0; i < modeCount; i++ )
	{
		ErrorCheck( mInterface->EnumAdapterModes( adapter, backbufferFormat, i, &modes[j] ), TEXT( "Enumerating Display Modes Failed" ) );
		switch( aspect )
		{
			case FourThree:
				if( ( (float)modes[j].Width / (float)modes[j].Height ) == ( 4.0f / 3.0f ) )
				{
					if( modes[j-1].Width != modes[j].Width || modes[j-1].Width != modes[j].Width || modes[j-1].RefreshRate != modes[j].RefreshRate )
					{
						sprintf( temp, "%u x %u, %uhz", modes[j].Width, modes[j].Height, modes[j].RefreshRate );
						window.AddDisplayOption( temp );
						j++;
					}
				}
				break;
			case SixteenNine:
				if( ( (float)modes[j].Width / (float)modes[j].Height ) == ( 16.0f / 9.0f ) )
				{
					if( modes[j-1].Width != modes[j].Width || modes[j-1].Width != modes[j].Width || modes[j-1].RefreshRate != modes[j].RefreshRate )
					{
						sprintf( temp, "%u x %u, %uhz", modes[j].Width, modes[j].Height, modes[j].RefreshRate );
						window.AddDisplayOption( temp );
						j++;
					}
				}
				break;
			case SixteenTen:
				if( ( (float)modes[j].Width / (float)modes[j].Height ) == ( 16.0f / 10.0f ) )
				{
					if( modes[j-1].Width != modes[j].Width || modes[j-1].Width != modes[j].Width || modes[j-1].RefreshRate != modes[j].RefreshRate )
					{
						sprintf( temp, "%u x %u, %uhz", modes[j].Width, modes[j].Height, modes[j].RefreshRate );
						window.AddDisplayOption( temp );
						j++;
					}
				}
				break;
		}
	}
}

void Graphics::SelectAspect43( void )
{
	aspect = FourThree;
}

void Graphics::SelectAspect169( void )
{
	aspect = SixteenNine;
}

void Graphics::SelectAspect1610( void )
{
	aspect = SixteenTen;
}

void Graphics::SelectResolution( const int& index )
{
	refresh = modes[index].RefreshRate;
	ChangeResolution( modes[index].Width, modes[index].Height );
	ChangeClientSize( modes[index].Width, modes[index].Height );
}