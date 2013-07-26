#include "Graphics/Graphics.h"
#include "System/System.h"
#include "Window/WindowMS.h"

Graphics::Graphics( System &s ) : log("Graphics"), system( s ), settings( system.settings ), window( system.window ), mInterface( NULL ), mDevice( NULL ),
	showCursorFullscreen( false ), forceClientToRes( true ), adapter( D3DADAPTER_DEFAULT ), adapters( NULL )
{
	fullscreen = settings.client.fullscreen;
	xResolution = settings.client.xResolution;
	yResolution = settings.client.yResolution;
	xWindowPosition = settings.window.x;
	yWindowPosition = settings.window.y;
	xWindowSize = settings.window.width;
	yWindowSize = settings.window.height;
	clientWidth = xResolution; //Plus one?
	clientHeight = yResolution; //Plus one?
	xClientPos = settings.client.x;
	yClientPos = settings.client.y;

	deviceType = D3DDEVTYPE_HAL;
	behaviourFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	backbufferFormat = D3DFMT_A8R8G8B8;
	depthFormat = D3DFMT_D16;
	bufferCount = 1;
	qualityAA = 0;
	AA = D3DMULTISAMPLE_NONE;
	refresh = 0;
	aspect = FourThree;
	mode = 0;
	modes = NULL;
}

Graphics::~Graphics( void )
{
	settings.client.fullscreen = fullscreen;
	settings.client.xResolution = xResolution;
	settings.client.yResolution = yResolution;

	CleanUp();
}

LPDIRECT3D9 const Graphics::Interface( void ) const
{
	return mInterface;
}

LPDIRECT3DDEVICE9 const Graphics::Device( void ) const
{
	return mDevice;
}

bool Graphics::IsDeviceLost( void ) const
{
	return ( deviceState != D3D_OK );
}

void Graphics::Initialise( void )
{
	CreateInterface();

	GetAdapters();

	SetParameters();

	CreateDevice();

	SetRenderStates();
}

void Graphics::SetRenderStates( void )
{
	ErrorCheck( mDevice->SetRenderState( D3DRS_LIGHTING, false ), TEXT( "Setting Lighting State Failed" ) );
	ErrorCheck( mDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW ), TEXT( "Setting Culling Mode Failed" ) );
	ErrorCheck( mDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE ), TEXT( "Enabling Depth Testing" ) );
	ErrorCheck( mDevice->SetRenderState( D3DRS_COLORVERTEX, TRUE ), TEXT( "Enabling Vertex Colours" ) );
}

void Graphics::SetDebugStates( void )
{
	ErrorCheck( mDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME ), TEXT( "Setting Wireframe mode" ) );
	ErrorCheck( mDevice->SetRenderState( D3DRS_LIGHTING, false ), TEXT( "Setting Lighting State Failed" ) );
	ErrorCheck( mDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ), TEXT( "Setting Culling Mode Failed" ) );
}

void Graphics::SetStandardStates( void )
{
	ErrorCheck( mDevice->SetRenderState( D3DRS_LIGHTING, false ), TEXT( "Setting Lighting State Failed" ) );
	ErrorCheck( mDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW ), TEXT( "Setting Culling Mode Failed" ) );
	ErrorCheck( mDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE ), TEXT( "Enabling Depth Testing" ) );
	ErrorCheck( mDevice->SetRenderState( D3DRS_COLORVERTEX, TRUE ), TEXT( "Enabling Vertex Colours" ) );
}

void Graphics::CleanUp( void )
{
	if( adapters != NULL )
	{
		delete[] adapters;
		adapters = NULL;
	}
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

void Graphics::DoChecks( void )
{
	CheckDevice();
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
	SetParameters();

	system.sceneManager.OnLost();

	if( Reset() )
	{
		system.sceneManager.OnRecover();
		SetRenderStates();
	}
}

void Graphics::SetFullscreen( void )
{
	if( !fullscreen )
	{
		fullscreen = true;

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
		clientWidth = width;
		clientHeight = height;
		window.SetClientSize( width, height );
		window.Update();
	}
}

void Graphics::SetClientPosition( const int& x, const int& y )
{
	if( !fullscreen )
	{
		xClientPos = x;
		yClientPos = y;
		window.SetClientPosition( x, y );
		window.Update();
	}
}

void Graphics::SetResolution( const int& width, const int& height )
{
	xResolution = width;
	yResolution = height;

	SetClientSize( width, height );

	Refresh();

	if( xResolution != xWindowSize )
	{
		if( xResolution < xWindowSize )
		{
			log.Message( "WARNING: Stretching image on x-axis", true );
		}
		if( xResolution > xWindowSize )
		{
			log.Message( "WARNING: Shrinking image on x-axis", true );
		}
	}

	if( yResolution != yWindowSize )
	{
		if( yResolution < yWindowSize )
		{
			log.Message( "WARNING: Stretching image on y-axis", true );
		}
		if( yResolution > yWindowSize )
		{
			log.Message( "WARNING: Shrinking image on y-axis", true );
		}
	}
}

void Graphics::ChangeView( void )
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

void Graphics::ErrorCheck( HRESULT result, LPCTSTR info )
{
	if( result == D3D_OK )
		return;
	
	LPTSTR text;

	switch( result )
	{
		case D3DERR_INVALIDCALL:
			text = TEXT( "D3DERR_INVALIDCALL: Invalid Call or Parameter!" );
			break;
		case D3DERR_NOTAVAILABLE:
			text = TEXT( "D3DERR_NOTAVAILABLE: Parameter not supported!" );
			break;
		case D3DERR_OUTOFVIDEOMEMORY:
			text = TEXT( "D3DERR_OUTOFVIDEOMEMORY: Out of Video Memory!" );
			break;
		case  D3DXERR_INVALIDDATA:
			text = TEXT( "D3DXERR_INVALIDDATA: Invalid data passed to D3DX function!" );
			break;
		case D3DERR_DEVICELOST:
			text = TEXT( "D3DERR_DEVICELOST: The device is lost!" );
			break;
		case D3DERR_DRIVERINTERNALERROR:
			text = TEXT( "D3DERR_DRIVERINTERNALERROR: Internal Driver Error!" );
			break;
		case E_OUTOFMEMORY:
			text = TEXT( "E_OUTOFMEMORY: Out of Memory" );
		default:
			text = new TCHAR[128];
			wsprintf( text, TEXT("Unknown Error: %X"), result );
			break;
	}

	LPTSTR error = new TCHAR[256];
	wsprintf( error, TEXT( "Graphics: %s\r\n%s" ), info, text );

	throw( error );
}