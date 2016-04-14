//Implementation of test scene
#include "SceneManager/SplashScreen.h"
#include "SceneManager/SceneManager.h"
#include "System/System.h"

bool regSplashScreen = RegisterScene<SplashScreen>( "SplashScreen" );

SplashScreen::SplashScreen( System &s, const char* n ) : IScene( s, n ), vertexBuffer( NULL ), texture( NULL ), time( 0.0f )
{
	tuMin = 0.0f;
	tuMax = 1.0f;
	tvMin = 0.0f;
	tvMax = 1.0f;

	timeStep = 0.5f;
	maxTime = 60.0f;
}

SplashScreen::~SplashScreen( void )
{
}

void SplashScreen::Load( void )
{
	if( !loaded )
	{
		Vertex2dTx vertices[] =
		{
			//Hey dumbass, these are SCREEN SPACE COORDINATES, ORIGIN IS TOP-LEFT >:|
			{ 0.0f, 0.0f, 0.0f, 1.0f, tuMin, tvMin },
			{ (float)settings.GetInteger( "display/xResolution" ), 0.0f, 0.0f, 1.0f, tuMax, tvMin },
			{ 0.0f, (float)settings.GetInteger( "display/yResolution" ), 0.0f, 1.0f, tuMin, tvMax },
			{ (float)settings.GetInteger( "display/xResolution" ), (float)settings.GetInteger( "display/yResolution" ), 0.0f, 1.0f, tuMax, tvMax },
		};

		graphics.ErrorCheck( graphics.Device()->CreateVertexBuffer( 4*sizeof(Vertex2dTx), 0, Vertex2dTx::format, D3DPOOL_MANAGED, &vertexBuffer, NULL ), "SplashScreen: Vertex Buffer creation FAILED" );

		graphics.ErrorCheck( D3DXCreateTextureFromFile( graphics.Device(), TEXT( "StandardResources/SplashScreen.png" ), &texture ), "SplashScreen: Texture creation FAILED" );

		VOID* pVoid;

		graphics.ErrorCheck( vertexBuffer->Lock( 0, 0, (void**)&pVoid, 0 ), "SplashScreen: Error locking vertex buffer" );
		memcpy( pVoid, vertices, sizeof(vertices) );
		graphics.ErrorCheck( vertexBuffer->Unlock(), "SplashScreen: Error unlocking vertex buffer" );
	}

	//Always set state and report
	loaded = true;
	//system.sceneManager.ReportFinishedLoading( this );
}

void SplashScreen::Unload( void )
{
	if( loaded )
	{
		log.Message( "Unloading", true );
		if( vertexBuffer != NULL )
		{
			vertexBuffer->Release();
			vertexBuffer = NULL;
		}

		if( texture != NULL )
		{
			texture->Release();
			texture = NULL;
		}
	}

	//Always set state and report
	loaded = false;
	//system.sceneManager.ReportFinishedUnloading( this );
}

void SplashScreen::OnLost( void )
{
	if( !lost )
	{
		if( vertexBuffer != NULL )
		{
			vertexBuffer->Release();
			vertexBuffer = NULL;
		}

		//Textures are in MANAGED pool
	}
	lost = true;
}

void SplashScreen::OnRecover( void )
{
	if( lost )
	{
		graphics.ErrorCheck( graphics.Device()->CreateVertexBuffer( 4*sizeof(Vertex2dTx), 0, Vertex2dTx::format, D3DPOOL_MANAGED, &vertexBuffer, NULL ), "SplashScreen: Vertex Buffer creation FAILED" );

		//Textures are in MANAGED pool

		Vertex2dTx vertices[] =
		{
			//Hey dumbass, these are SCREEN SPACE COORDINATES, ORIGIN IS TOP-LEFT >:|
			{ 0.0f, 0.0f, 0.0f, 1.0f, tuMin, tvMin },
			{ (float)settings.GetInteger( "client/xResolution" ), 0.0f, 0.0f, 1.0f, tuMax, tvMin },
			{ 0.0f, (float)settings.GetInteger( "client/yResolution" ), 0.0f, 1.0f, tuMin, tvMax },
			{ (float)settings.GetInteger( "client/xResolution" ), (float)settings.GetInteger( "client/yResolution" ), 0.0f, 1.0f, tuMax, tvMax },
		};

		VOID* pVoid;

		graphics.ErrorCheck( vertexBuffer->Lock( 0, 0, (void**)&pVoid, 0 ), "SplashScreen: Error locking vertex buffer again" );
		memcpy( pVoid, vertices, sizeof(vertices) );
		graphics.ErrorCheck( vertexBuffer->Unlock(), "SplashScreen: Error unlocking vertex buffer again" );
	}
	lost = false;
}

void SplashScreen::MainLoop( void )
{
	if( time > maxTime )
	{
		system.sceneManager.ChangeScene( "TestScene" );
	}
	else
	{
		time += timeStep;
	}
}

void SplashScreen::RenderMain( void )
{
	// clear the window to a deep blue
	graphics.Device()->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_LIGHTING, false ), "Setting Lighting State Failed" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW ), "Setting Culling Mode Failed" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE ), "Enabling Depth Testing" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_COLORVERTEX, TRUE ), "Enabling Vertex Colours" );

    graphics.Device()->BeginScene();    // begins the 3D scene

	graphics.Device()->SetFVF(Vertex2dTx::format);

	graphics.Device()->SetTexture( 0, texture );
	graphics.Device()->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
	graphics.Device()->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	graphics.Device()->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE); 
	
	graphics.Device()->SetStreamSource( 0, vertexBuffer, 0, sizeof(Vertex2dTx) );

    graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	graphics.Device()->EndScene();    // ends the 3D scene

    graphics.Device()->Present( NULL, NULL, NULL, NULL );   // displays the created frame on the screen
}

void SplashScreen::Start( void )
{
}

void SplashScreen::End( void )
{
}