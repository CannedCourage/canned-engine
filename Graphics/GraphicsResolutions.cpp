#include "Graphics/Graphics.h"
#include "Window/WindowMS.h"

void Graphics::SelectAspect( const int& index )
{
	switch( index )
	{
		case 0:
			SelectAspect43();
			break;
		case 1:
			SelectAspect169();
			break;
		case 2:
			SelectAspect1610();
			break;
		default:
			SelectAspect43();
			break;
	}

	GetModesFromDevice();
}

void Graphics::GetModesFromDevice( void )
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

	log.Message( "Checking Modes" );

	for( UINT i = 0; i < modeCount; i++ )
	{
		ErrorCheck( mInterface->EnumAdapterModes( adapter, backbufferFormat, i, &modes[j] ), TEXT( "Enumerating Display Modes Failed" ) );
		sprintf( temp, "%u x %u, %uhz", modes[j].Width, modes[j].Height, modes[j].RefreshRate );
		log.Message( temp );
		switch( aspect )
		{
			case FourThree:
				if( ( (float)modes[j].Width / (float)modes[j].Height ) == ( 4.0f / 3.0f ) )
				{
					//if( modes[j-1].Width != modes[j].Width || modes[j-1].Width != modes[j].Width || modes[j-1].RefreshRate != modes[j].RefreshRate )
					//{
						sprintf( temp, "%u x %u, %uhz", modes[j].Width, modes[j].Height, modes[j].RefreshRate );
						log.Message( temp );
						j++;
					//}
				}
				break;
			case SixteenNine:
				if( ( (float)modes[j].Width / (float)modes[j].Height ) == ( 16.0f / 9.0f ) )
				{
					//if( modes[j-1].Width != modes[j].Width || modes[j-1].Width != modes[j].Width || modes[j-1].RefreshRate != modes[j].RefreshRate )
					//{
						sprintf( temp, "%u x %u, %uhz", modes[j].Width, modes[j].Height, modes[j].RefreshRate );
						log.Message( temp );
						j++;
					//}
				}
				break;
			case SixteenTen:
				if( ( (float)modes[j].Width / (float)modes[j].Height ) == ( 16.0f / 10.0f ) )
				{
					//if( modes[j-1].Width != modes[j].Width || modes[j-1].Width != modes[j].Width || modes[j-1].RefreshRate != modes[j].RefreshRate )
					//{
						sprintf( temp, "%u x %u, %uhz", modes[j].Width, modes[j].Height, modes[j].RefreshRate );
						log.Message( temp );
						j++;
					//}
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
	SetResolution( modes[index].Width, modes[index].Height );
}