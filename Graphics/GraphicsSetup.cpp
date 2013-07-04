#include "Graphics\Graphics.h"

void Graphics::CreateInterface( void )
{
	HWND hWnd = window->getHandle();

    mInterface = Direct3DCreate9( D3D_SDK_VERSION );

	if( mInterface == NULL )
	{
		throw( TEXT( "Graphics: Failed to create direct3D interface" ) );
	}
}

void Graphics::GetAdapters( void )
{
	adapterCount = mInterface->GetAdapterCount();

	if( adapterCount > 0 )
	{
		window->AdapterDialog();
	}
	else
	{
		throw( TEXT( "No suitable adapter" ) );
	}

	adapters = new D3DADAPTER_IDENTIFIER9[adapterCount];

	for( UINT i = 0; i < adapterCount; i++ )
	{
		mInterface->GetAdapterIdentifier( i, 0, &( adapters[i] ) );
		window->AddAdapterOption( adapters[i].Description );
	}
}

void Graphics::SelectAdapter( int index )
{
	index--;	//Subtract one to align combobox index with adapter number, assumes nonsorted list and corresponding adapter list

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
	switch( index )
	{
		case 1:
			SetBufferFormat32();
			break;
		case 2:
			SetBufferFormat24();
			break;
		case 3:
			SetBufferFormat16();
			break;
		default:
			SetBufferFormat32();
			break;
	}
}

void Graphics::SelectDepthFormat( const int& index )
{
	switch( index )
	{
		case 1:
			SetDepthBufferD32();
			break;
		case 2:
			SetDepthBufferD24S8();
			break;
		case 3:
			SetDepthBufferD15S1();
			break;
		case 4:
			SetDepthBufferD16();
			break;
		default:
			SetDepthBufferD16();
			break;
	}
}

void Graphics::SelectMultisample( const int& n )
{
	switch( n )
	{
		case 1:
			SetMultisampleOff();
			break;
		case 2:
			SetMultisample2();
			break;
		case 3:
			SetMultisample4();
			break;
		case 4:
			SetMultisample8();
			break;
		case 5:
			SetMultisample16();
			break;
		default:
			SetMultisampleOff();
			break;
	}
}

void Graphics::CheckDeviceCaps( void )
{
	ErrorCheck( mInterface->GetDeviceCaps( adapter, deviceType, &caps ), TEXT( "Checking device caps" ) );

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
	HWND hWnd = window->getHandle();

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
	fullParameters.MultiSampleQuality = 0;						//Anti-Aliasing quality
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
	windowParameters.MultiSampleQuality = qualityAA;					//Anti-Aliasing quality
	windowParameters.MultiSampleType = AA;								//No Anti-Aliasing
	windowParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;	//Present back buffer once per screen refresh
	windowParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;				//Discard old frames
	windowParameters.Windowed = true;

	if( windowed )
	{
		presentParameters = &( windowParameters );
	}
	else
	{
		presentParameters = &( fullParameters );
	}
}

void Graphics::SetBufferFormat16( void )
{
	if( mInterface->CheckDeviceType( adapter, D3DDEVTYPE_HAL, D3DFMT_R5G6B5, D3DFMT_R5G6B5, windowed ) != D3D_OK )
	{
		if( mInterface->CheckDeviceType( adapter, D3DDEVTYPE_REF, D3DFMT_R5G6B5, D3DFMT_R5G6B5, windowed ) )
		{
			throw( TEXT( "Graphics: No appropriate format or device could be found" ) );
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

void Graphics::SetBufferFormat24( void )
{
	if( mInterface->CheckDeviceType( adapter, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, windowed ) != D3D_OK )
	{
		SetBufferFormat16();
	}
	else
	{
		deviceType = D3DDEVTYPE_HAL;
		backbufferFormat = D3DFMT_X8R8G8B8;

		CheckDeviceCaps();
	}
}

void Graphics::SetBufferFormat32( void )
{
	if( mInterface->CheckDeviceType( adapter, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, D3DFMT_A8R8G8B8, windowed ) != D3D_OK )
	{
		SetBufferFormat24();
	}
	else
	{
		deviceType = D3DDEVTYPE_HAL;
		backbufferFormat = D3DFMT_A8R8G8B8;

		CheckDeviceCaps();
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
		throw( TEXT( "Graphics: No valid depth format available" ) );
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

void Graphics::SetMultisample2( void )
{
	if( mInterface->CheckDeviceMultiSampleType( adapter, deviceType, backbufferFormat, windowed, D3DMULTISAMPLE_2_SAMPLES, &qualityAA ) != D3D_OK )
	{
		SetMultisampleOff();
	}
	else
	{
		AA = D3DMULTISAMPLE_2_SAMPLES;
		qualityAA = 0;
	}
}

void Graphics::SetMultisample4( void )
{
	if( mInterface->CheckDeviceMultiSampleType( adapter, deviceType, backbufferFormat, windowed, D3DMULTISAMPLE_4_SAMPLES, &qualityAA ) != D3D_OK )
	{
		SetMultisample2();
	}
	else
	{
		AA = D3DMULTISAMPLE_4_SAMPLES;
		qualityAA = 0;
	}
}

void Graphics::SetMultisample8( void )
{
	if( mInterface->CheckDeviceMultiSampleType( adapter, deviceType, backbufferFormat, windowed, D3DMULTISAMPLE_8_SAMPLES, &qualityAA ) != D3D_OK )
	{
		SetMultisample4();
	}
	else
	{
		AA = D3DMULTISAMPLE_8_SAMPLES;
		qualityAA = 0;
	}
}

void Graphics::SetMultisample16( void )
{
	if( mInterface->CheckDeviceMultiSampleType( adapter, deviceType, backbufferFormat, windowed, D3DMULTISAMPLE_16_SAMPLES, &qualityAA ) != D3D_OK )
	{
		SetMultisample8();
	}
	else
	{
		AA = D3DMULTISAMPLE_16_SAMPLES;
		qualityAA = 0;
	}
}

void Graphics::CreateDevice( void )
{
	HWND hWnd = window->getHandle();

	ErrorCheck( mInterface->CreateDevice(adapter,
                      deviceType,
                      hWnd,
                      behaviourFlags,
                      presentParameters,
                      &mDevice),
					  TEXT( "Failed at device creation" ) );
}