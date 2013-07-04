//Implementation of test scene
#include "SceneManager\SplashScreen.h"
#include "SceneManager\SceneManager.h"

SplashScreen::SplashScreen( const char* n ) : IScene(n), vertexBuffer( NULL ), texture( NULL ), time( 0.0f )
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
			{ (float)settings->client.xResolution, 0.0f, 0.0f, 1.0f, tuMax, tvMin },
			{ 0.0f, (float)settings->client.yResolution, 0.0f, 1.0f, tuMin, tvMax },
			{ (float)settings->client.xResolution, (float)settings->client.yResolution, 0.0f, 1.0f, tuMax, tvMax },
		};

		graphics->ErrorCheck( device->CreateVertexBuffer( 4*sizeof(Vertex2dTx), 0, Vertex2dTx::format, D3DPOOL_MANAGED, &vertexBuffer, NULL ), TEXT( "SplashScreen: Vertex Buffer creation FAILED" ) );

		graphics->ErrorCheck( D3DXCreateTextureFromFile( device, TEXT( "StandardResources/SplashScreen.png" ), &texture ), TEXT( "SplashScreen: Texture creation FAILED" ) );

		VOID* pVoid;

		graphics->ErrorCheck( vertexBuffer->Lock( 0, 0, (void**)&pVoid, 0 ), TEXT( "SplashScreen: Error locking vertex buffer" ) );
		memcpy( pVoid, vertices, sizeof(vertices) );
		graphics->ErrorCheck( vertexBuffer->Unlock(), TEXT( "SplashScreen: Error unlocking vertex buffer" ) );
	}

	//Always set state and report
	loaded = true;
	manager->ReportFinishedLoading( this );
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
	manager->ReportFinishedUnloading( this );
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
		graphics->ErrorCheck( device->CreateVertexBuffer( 4*sizeof(Vertex2dTx), 0, Vertex2dTx::format, D3DPOOL_MANAGED, &vertexBuffer, NULL ), TEXT( "SplashScreen: Vertex Buffer creation FAILED" ) );

		//Textures are in MANAGED pool

		Vertex2dTx vertices[] =
		{
			//Hey dumbass, these are SCREEN SPACE COORDINATES, ORIGIN IS TOP-LEFT >:|
			{ 0.0f, 0.0f, 0.0f, 1.0f, tuMin, tvMin },
			{ (float)settings->client.xResolution, 0.0f, 0.0f, 1.0f, tuMax, tvMin },
			{ 0.0f, (float)settings->client.yResolution, 0.0f, 1.0f, tuMin, tvMax },
			{ (float)settings->client.xResolution, (float)settings->client.yResolution, 0.0f, 1.0f, tuMax, tvMax },
		};

		VOID* pVoid;

		graphics->ErrorCheck( vertexBuffer->Lock( 0, 0, (void**)&pVoid, 0 ), TEXT( "SplashScreen: Error locking vertex buffer again" ) );
		memcpy( pVoid, vertices, sizeof(vertices) );
		graphics->ErrorCheck( vertexBuffer->Unlock(), TEXT( "SplashScreen: Error unlocking vertex buffer again" ) );
	}
	lost = false;
}

void SplashScreen::FadeIn( void )
{
	SetState(update);
}

void SplashScreen::MainLoop( void )
{
	if( time > maxTime )
	{
		manager->ChangeScene( new TestScene() );
	}
	else
	{
		time += timeStep;
	}
}

void SplashScreen::FadeOut( void )
{
	Unload();
}

void SplashScreen::RenderIn( void )
{
	device->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
	device->BeginScene();    // begins the 3D scene
	device->EndScene();    // ends the 3D scene
	device->Present( NULL, NULL, NULL, NULL );   // displays the created frame on the screen
}

void SplashScreen::RenderMain( void )
{
	// clear the window to a deep blue
	device->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

    device->BeginScene();    // begins the 3D scene

	device->SetFVF(Vertex2dTx::format);

	device->SetTexture( 0, texture );
	device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
	device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE); 
	
	device->SetStreamSource( 0, vertexBuffer, 0, sizeof(Vertex2dTx) );

    device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	device->EndScene();    // ends the 3D scene

    device->Present( NULL, NULL, NULL, NULL );   // displays the created frame on the screen
}

void SplashScreen::RenderOut( void )
{
	device->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
	device->BeginScene();    // begins the 3D scene
	device->EndScene();    // ends the 3D scene
	device->Present( NULL, NULL, NULL, NULL );   // displays the created frame on the screen
}