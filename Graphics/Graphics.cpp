#include "Graphics/Graphics.h"
#include "System/System.h"
#include "Window/WindowMS.h"

#include <iostream>
#include <sstream>

using std::string;
using std::ostringstream;

//TODO: Replace window references with message system

Graphics::Graphics( System &s ) : 	log("Graphics"), system( s ), settings( system.settings ), window( system.window ), mInterface( NULL ), mDevice( NULL ),
									presentParameters( NULL ), adapterCount( 0 ), modeCount( 0 ), adapter( D3DADAPTER_DEFAULT ), mode( 0 ),
									deviceType( D3DDEVTYPE_HAL ), backbufferFormat( D3DFMT_A8R8G8B8 ), depthFormat( D3DFMT_D16 ),
									behaviourFlags( D3DCREATE_HARDWARE_VERTEXPROCESSING ), qualityAA( 0 ), bufferCount( 1 ), AA( D3DMULTISAMPLE_NONE ), modes( NULL ),
									refresh( 0 ), aspect( FourThree ), showCursorFullscreen( false ), dModes(), adapterList()
{
	ReadSettings();
}

Graphics::~Graphics( void )
{
	WriteSettings();
}

LPDIRECT3D9 const Graphics::Interface( void ) const
{
	return mInterface;
}

LPDIRECT3DDEVICE9 const Graphics::Device( void ) const
{
	return mDevice;
}

bool Graphics::IsDeviceLost( void )
{
	CheckDevice();

	return ( deviceState != D3D_OK );
}

void Graphics::Init( void )
{
	CreateInterface();

	GetAdaptersFromInterface();

	GetModesFromInterface();

	ReadSettings();

	ApplySettings();

	SetParameters();

	CreateDevice();
}

void Graphics::SetDebugStates( void )
{
	ErrorCheck( mDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME ), TEXT( "Setting Wireframe mode" ) );
	ErrorCheck( mDevice->SetRenderState( D3DRS_LIGHTING, false ), TEXT( "Setting Lighting State Failed" ) );
	ErrorCheck( mDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ), TEXT( "Setting Culling Mode Failed" ) );
}

void Graphics::CleanUp( void )
{
	if( modes !=  NULL )
	{
		delete[] modes;
		modes = NULL;
	}

    if( mDevice != NULL )
	{
		mDevice->Release();
		mDevice = NULL;
	}

    if( mInterface != NULL )
	{
		mInterface->Release();
		mInterface = NULL;
	}
}

void Graphics::CheckDevice( void )
{
	deviceState = ( mDevice->TestCooperativeLevel() );
}

bool Graphics::Reset( void )
{
	log.Message( "Attempting Reset", true );
	HRESULT result = mDevice->Reset( presentParameters );

	if( result == D3D_OK )
	{
		log.Message( "Device reset", true );
	}
	else
	{
		log.Message( "Device not ok", true );
	}

	if( result == D3DERR_DEVICELOST )
	{
		log.Message( "Device Lost", true );
	}

	if( result == D3DERR_DEVICENOTRESET )
	{
		log.Message( "Device not reset", true );
	}

	return ( D3D_OK == result );
}

void Graphics::Refresh( void )
{
	ReadSettings();

	ApplySettings();

	SetParameters();

	//TODO: Replace with message system?
	system.sceneManager.OnLost();

	if( Reset() )
	{
		log.Message( "Recovering", true );
		system.sceneManager.OnRecover();
	}
}

void Graphics::ReadSettings( void )
{
	fullscreen = settings.GetBool( "display/fullscreen" );
	xResolution = settings.GetInteger( "display/xResolution" );
	yResolution = settings.GetInteger( "display/yResolution" );
	aspect = (ASPECT)settings.GetInteger( "display/aspect" );
	refresh = settings.GetInteger( "display/refresh" );
	AA = (D3DMULTISAMPLE_TYPE)settings.GetInteger( "display/multisample" );
	mode = settings.GetInteger( "display/displayMode" );
	backbufferFormat = (D3DFORMAT)settings.GetInteger( "display/bufferFormat" );
	depthFormat = (D3DFORMAT)settings.GetInteger( "display/depthFormat" );
	adapter = settings.GetInteger( "display/adapter" );
}

void Graphics::WriteSettings( void )
{
	settings.SetBool( "display/fullscreen", fullscreen );
	settings.SetInteger( "display/xResolution", xResolution );
	settings.SetInteger( "display/yResolution", yResolution );
	settings.SetInteger( "display/aspect", aspect );
	settings.SetInteger( "display/refresh", refresh );
	settings.SetInteger( "display/multisample", AA );
	settings.SetInteger( "display/displayMode", mode );
	settings.SetInteger( "display/bufferFormat", backbufferFormat );
	settings.SetInteger( "display/depthFormat", depthFormat );
	settings.SetInteger( "display/bufferCount", bufferCount );
	settings.SetInteger( "display/adapter", adapter );
}

void Graphics::ApplySettings( void )
{
	SelectAdapter( adapter );

	if( bufferCount < 0 || bufferCount > 2 ){ DoubleBuffer(); }

	SelectBufferFormat( backbufferFormat );

	SelectDepthFormat( depthFormat );

	SelectMultisample( AA );
}

void Graphics::SetFullscreen( void )
{
	if( !fullscreen )
	{
		fullscreen = true;

		WriteSettings();

		Refresh();

		if( showCursorFullscreen )
		{
			window.CursorVisible( true );
		}
		else
		{
			window.CursorVisible( false );
		}
	}
	else
	{
		log.Message( "Already Fullscreen, command ignored" );
	}
}

void Graphics::SetWindowed( void )
{
	if( fullscreen )
	{
		fullscreen = false;

		WriteSettings();

		Refresh();

		window.Update();

		window.CursorVisible( true );
	}
	else
	{
		log.Message( "Already Windowed, command ignored" );
	}
}

void Graphics::SetClientSize( const int& width, const int& height )
{
	if( !fullscreen )
	{
		window.SetClientSize( width, height );
		window.Update();
	}
}

void Graphics::ToggleFullscreen( void )
{
	if( !fullscreen )
	{
		SetFullscreen();
	}
	else
	{
		SetWindowed();
	}
}

void Graphics::ErrorCheck( HRESULT result, const char* const info )
{
	if( result == D3D_OK )
		return;
	
	string text("");

	switch( result )
	{
		case D3DERR_INVALIDCALL:
			text = "D3DERR_INVALIDCALL: Invalid Call or Parameter!";
			break;
		case D3DERR_NOTAVAILABLE:
			text = "D3DERR_NOTAVAILABLE: Parameter not supported!";
			break;
		case D3DERR_OUTOFVIDEOMEMORY:
			text = "D3DERR_OUTOFVIDEOMEMORY: Out of Video Memory!";
			break;
		case  D3DXERR_INVALIDDATA:
			text = "D3DXERR_INVALIDDATA: Invalid data passed to D3DX function!";
			break;
		case D3DERR_DEVICELOST:
			text = "D3DERR_DEVICELOST: The device is lost!";
			break;
		case D3DERR_DRIVERINTERNALERROR:
			text = "D3DERR_DRIVERINTERNALERROR: Internal Driver Error!";
			break;
		case E_OUTOFMEMORY:
			text = "E_OUTOFMEMORY: Out of Memory";
		default:
			ostringstream s;
			s << "Unknown Error: " << result;
			text = s.str();
			break;
	}

	ostringstream e;

	e << "Graphics: " << info << "\r\n" << text;

	string error = e.str();

	throw( error.c_str() );
}