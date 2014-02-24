#include "Graphics/Graphics.h"
#include "Window/WindowMS.h"

void Graphics::CreateInterface( void )
{
	WindowMS& winMS = static_cast<WindowMS&>(window);

	HWND hWnd = winMS.getHandle();

    mInterface = Direct3DCreate9( D3D_SDK_VERSION );

	if( mInterface == NULL )
	{
		throw( "Graphics: Failed to create direct3D interface" );
	}
}

void Graphics::GetAdaptersFromInterface( void )
{
	adapterCount = mInterface->GetAdapterCount();

	if( adapterCount > 0 )
	{
		//OK
	}
	else
	{
		throw( "No suitable adapter" );
	}

	D3DADAPTER_IDENTIFIER9 ident;

	for( UINT i = 0; i < adapterCount; i++ )
	{
		mInterface->GetAdapterIdentifier( i, 0, &ident );

		adapterDesc a( ident.Description, i );

		adapterList.push_back( a );

		log.Message( ident.Description, i );
	}
}

const std::vector<adapterDesc>& Graphics::GetAdapters( void )
{
	return adapterList;
}

void Graphics::SelectAdapter( const int& index )
{
	if( index >= 0 )
	{
		adapter = index;
	}
	else
	{
		adapter = 0;
	}
}

void Graphics::SelectBufferFormat( const int& index )
{
	if( ( index < 21 ) || ( index > 23 ) )
	{
		SetBufferFormat16();
		return;
	}

	if( mInterface->CheckDeviceType( adapter, D3DDEVTYPE_HAL, (D3DFORMAT)index, (D3DFORMAT)index, !fullscreen ) != D3D_OK )
	{
		SelectBufferFormat( index + 1 );
	}
	else
	{
		deviceType = D3DDEVTYPE_HAL;
		backbufferFormat = (D3DFORMAT)index;

		CheckDeviceCaps();
	}
}

void Graphics::SetBufferFormat16( void )
{
	if( mInterface->CheckDeviceType( adapter, D3DDEVTYPE_HAL, D3DFMT_R5G6B5, D3DFMT_R5G6B5, !fullscreen ) != D3D_OK )
	{
		if( mInterface->CheckDeviceType( adapter, D3DDEVTYPE_REF, D3DFMT_R5G6B5, D3DFMT_R5G6B5, !fullscreen ) )
		{
			throw( "Graphics: No appropriate format or device could be found" );
		}
		else
		{
			deviceType = D3DDEVTYPE_REF;
			backbufferFormat = D3DFMT_R5G6B5;

			CheckDeviceCaps();
		}
	}
	else
	{
		deviceType = D3DDEVTYPE_HAL;
		backbufferFormat = D3DFMT_R5G6B5;

		CheckDeviceCaps();
	}
}

void Graphics::SelectDepthFormat( const int& index )
{
	switch( index )
	{
		case D32:
			SetDepthBufferD32();
			break;
		case D24S8:
			SetDepthBufferD24S8();
			break;
		case D15S1:
			SetDepthBufferD15S1();
			break;
		case D16:
			SetDepthBufferD16();
			break;
		default:
			SetDepthBufferD16();
			break;
	}
}

void Graphics::SelectMultisample( const int& samples )
{
	if( ( samples == 1 ) || ( samples < 0 ) )
	{
		SetMultisampleOff();
		return;
	}

	//fullscreen is inverted because the parameter needed is "windowed"
	if( mInterface->CheckDeviceMultiSampleType( adapter, deviceType, backbufferFormat, !fullscreen, (D3DMULTISAMPLE_TYPE)samples, &qualityAA ) != D3D_OK )
	{
		SelectMultisample( samples - 1 );
	}
	else
	{
		AA = (D3DMULTISAMPLE_TYPE)samples;
		qualityAA = 0;
	}
}

void Graphics::CheckDeviceCaps( void )
{
	ErrorCheck( mInterface->GetDeviceCaps( adapter, deviceType, &caps ), "Checking device caps" );

	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{
		behaviourFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		
		if( caps.DevCaps & D3DDEVCAPS_PUREDEVICE )
		{
			//behaviourFlags |= D3DCREATE_PUREDEVICE; //Enable after finished debugging, disables Get* calls, does not store state information
		}
	}
	else
	{
		behaviourFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
}

void Graphics::SetParameters( void )
{
	WindowMS& winMS = static_cast<WindowMS&>(window);

	HWND hWnd = winMS.getHandle();

	ZeroMemory( &fullParameters, sizeof( fullParameters ) );
	fullParameters.AutoDepthStencilFormat = depthFormat;				//16 bit depth buffer, 0 bit stencil buffer
	fullParameters.BackBufferCount = bufferCount;
	fullParameters.BackBufferFormat = backbufferFormat;					//Back buffer format to 32-bit
	fullParameters.BackBufferHeight = yResolution;
	fullParameters.BackBufferWidth = xResolution;
	fullParameters.EnableAutoDepthStencil = true;						//Enable depth buffer
	fullParameters.Flags = NULL;										//No flags
	fullParameters.FullScreen_RefreshRateInHz = refresh;				//Refresh rate, 0 asks DX to choose
	fullParameters.hDeviceWindow = hWnd;
	fullParameters.MultiSampleQuality = 0;								//Anti-Aliasing quality. Driver specific, should generally be 0.
	fullParameters.MultiSampleType = AA;								//No Anti-Aliasing
	fullParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//Present back buffer once per screen refresh
	fullParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;					//Discard old frames
	fullParameters.Windowed = false;

	ZeroMemory( &windowParameters, sizeof( windowParameters ) );
	windowParameters.AutoDepthStencilFormat = depthFormat;				//16 bit depth buffer, 0 bit stencil buffer
	windowParameters.BackBufferCount = bufferCount;
	windowParameters.BackBufferFormat = backbufferFormat;				//Back buffer format to 32-bit
	windowParameters.BackBufferHeight = yResolution;
	windowParameters.BackBufferWidth = xResolution;
	windowParameters.EnableAutoDepthStencil = true;						//Enable depth buffer
	windowParameters.Flags = NULL;										//No flags
	windowParameters.FullScreen_RefreshRateInHz = 0;					//Refresh rate, 0 asks DX to choose, must be 0 in windowed mode
	windowParameters.hDeviceWindow = hWnd;
	windowParameters.MultiSampleQuality = qualityAA;					//Anti-Aliasing quality. Driver specific, should generally be 0.
	windowParameters.MultiSampleType = AA;								//No Anti-Aliasing
	windowParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;	//Present back buffer once per screen refresh
	windowParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;				//Discard old frames
	windowParameters.Windowed = true;

	if( fullscreen )
	{
		presentParameters = &( fullParameters );
	}
	else
	{
		presentParameters = &( windowParameters );
	}
}

void Graphics::DoubleBuffer( void )
{
	bufferCount = 1;
}

void Graphics::TripleBuffer( void )
{
	bufferCount = 2;
}

void Graphics::SetDepthBufferD32( void )
{
	if( mInterface->CheckDeviceFormat( adapter, deviceType, backbufferFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) != D3D_OK )
	{
		SetDepthBufferD16();
	}
	else
	{
		depthFormat = D3DFMT_D32;
	}
}

void Graphics::SetDepthBufferD24S8( void )
{
	if( mInterface->CheckDeviceFormat( adapter, deviceType, backbufferFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) != D3D_OK )
	{
		SetDepthBufferD15S1();
	}
	else
	{
		depthFormat = D3DFMT_D24S8;
	}
}

void Graphics::SetDepthBufferD15S1( void )
{
	if( mInterface->CheckDeviceFormat( adapter, deviceType, backbufferFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) != D3D_OK )
	{
		SetDepthBufferD16();
	}
	else
	{
		depthFormat = D3DFMT_D15S1;
	}
}

void Graphics::SetDepthBufferD16( void )
{
	if( mInterface->CheckDeviceFormat( adapter, deviceType, backbufferFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 ) != D3D_OK )
	{
		throw( "Graphics: No valid depth format available" );
	}
	else
	{
		depthFormat = D3DFMT_D16;
	}
}

void Graphics::SetMultisampleOff( void )
{
	AA = D3DMULTISAMPLE_NONE;
	qualityAA = 0;
}

void Graphics::CreateDevice( void )
{
	WindowMS& winMS = static_cast<WindowMS&>(window);

	HWND hWnd = winMS.getHandle();

	mDevice = NULL;

	ErrorCheck( mInterface->CreateDevice(adapter,
                      deviceType,
                      hWnd,
                      behaviourFlags,
                      presentParameters,
                      &mDevice),
					  TEXT( "Failed at device creation" ) );
}