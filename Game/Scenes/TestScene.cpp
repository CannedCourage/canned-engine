#include "Game/Scenes/TestScene.h"
#include "System/System.h"

#define DRUMLOOP "w:/game/sounds/drumloop.wav"
#define TIGER "w:/game/meshes/tiger.x"
#define QMARK "w:/game/textures/qMark.bmp"
#define UVTEST "w:/game/textures/uvtemplate.bmp"

//Globals for scene variables?
ID3DXSprite* s = NULL;

TestScene::TestScene( System &s ) : IScene( s, "TestScene" ),
									cubeBuffer( NULL ),
									cubeTex( NULL ),
									font( NULL ),
									buffer( s ),
									keys( input ),
									logiMouse( input ),
									player1( input ),
									test1( input ),
									channel( NULL ),
									transforms(),
									meshes( s.graphics ),
									sprites( s.graphics, transforms ),
									animatedSprites( s.graphics, transforms ),
									AddTransform( transforms ),
									GetTransform( transforms ),
									AddSprite( sprites ),
									GetSprite( sprites ),
									AddAnimatedSprite( animatedSprites ),
									GetAnimatedSprite( animatedSprites )
{
	D3DXMatrixTranslation( &matWorld, 0, 0, 0 );
	D3DXMatrixLookAtLH( &matView, &(D3DXVECTOR3( 3.0f, 3.0f, -3.0f )), &(D3DXVECTOR3( 0.0f, 0.0f, 0.0f )), &(D3DXVECTOR3( 0.0f, 1.0f, 0.0f )) );
	D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 90.0f ), 
		(float)settings.GetInteger( "display/xResolution" )/(float)settings.GetInteger( "display/yResolution" ), 
		1.0f, 100.0f );

	test1.AddInput( &keys, &(PhysicalDevice::IsPressed), Keyboard::Keys::LEFT_CONTROL );
	test1.AddInput( &keys, &(PhysicalDevice::WentUp), Keyboard::Keys::LEFT_ALT );

	engine.AddProcess( &transforms, UPDATE );
	engine.AddProcess( &meshes, RENDER );
	engine.AddProcess( &sprites, RENDER );
	engine.AddProcess( &animatedSprites, RENDER );
}

TestScene::~TestScene( void )
{
	Unload();
}

void TestScene::Load( void )
{
	log.Message( "Loading", true );
	Vertex3dTx verts[] = { 
	// Front Face (1-2-3-4)
	{ -1.0f, 1.0f, -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, -1.0f, 1.0f, 0.0f }, { -1.0f, -1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, -1.0f, -1.0f, 1.0f, 1.0f },
	// Right Face (2-6-4-8)
	{ 1.0f, 1.0f, -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f, 0.0f }, { 1.0f, -1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, -1.0f, 1.0f, 1.0f, 1.0f },
	// Top Face (5-6-1-2)
	{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f, 0.0f }, { -1.0f, 1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, -1.0f, 1.0f, 1.0f },
	// Back Face (6-5-8-7)
	{ 1.0f, 1.0f, 1.0f, 0.0f, 0.0f }, { -1.0f, 1.0f, 1.0f, 1.0f, 0.0f }, { 1.0f, -1.0f, 1.0f, 0.0f, 1.0f }, { -1.0f, -1.0f, 1.0f, 1.0f, 1.0f },
	// Left Face (5-1-7-3)
	{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f }, { -1.0f, 1.0f, -1.0f, 1.0f, 0.0f }, { -1.0f, -1.0f, 1.0f, 0.0f, 1.0f }, { -1.0f, -1.0f, -1.0f, 1.0f, 1.0f },
	// Bottom Face (3-4-7-8)
	{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f }, { 1.0f, -1.0f, -1.0f, 1.0f, 0.0f }, { -1.0f, -1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, -1.0f, 1.0f, 1.0f, 1.0f }
	};

	graphics.ErrorCheck
	( 
		graphics.Device()->CreateVertexBuffer( 24*sizeof(Vertex3dTx), D3DUSAGE_WRITEONLY, Vertex3dTx::format, D3DPOOL_DEFAULT, &cubeBuffer, NULL ), 
		TEXT( "cubeBuffer creation failed" ) 
	);

	VOID* pVoid;
	graphics.ErrorCheck( cubeBuffer->Lock( 0, 0, (void**)&pVoid, 0 ), TEXT( "SplashScreen: Error locking cube buffer" ) );
	memcpy( pVoid, verts, sizeof(verts) );
	graphics.ErrorCheck( cubeBuffer->Unlock(), TEXT( "SplashScreen: Error unlocking cube buffer" ) );

	graphics.ErrorCheck
	( 
		D3DXCreateTextureFromFile( graphics.Device(), TEXT( "w:/game/textures/qMark.bmp" ), &cubeTex ), 
		TEXT( "Failed creating texture" ) 
	);

	//Meshes
	//IDEA: Eliminate Load* methods, only use Get* methods, asset manager loads if asset is not already loaded.
	assets.LoadMesh( TIGER );
	Entity& tiger = entityManager.New( "tiger" );
	meshes.AddMeshComponent( tiger.ID, assets.GetMesh( TIGER ) );

	assets.LoadSoundSample( DRUMLOOP );
	//sound.System()->playSound( assets.GetSound( DRUMLOOP ), 0, false, &channel );
	
	assets.LoadTexture( QMARK );
	assets.LoadTexture( UVTEST );

	Entity& qMark = entityManager.New( "qMark" );

	//TODO: Need to add delete* methods to remove components
	AddTransform( qMark );
	AddSprite( qMark, assets.GetTexture( QMARK ) ); //Processor could call asset manager instead?

	GetTransform( qMark ).scale.x = 512;
	GetTransform( qMark ).scale.y = 512;

	GetSprite( qMark ).TextureDimensions.right = 2.0f;
	GetSprite( qMark ).TextureDimensions.bottom = 2.0f;

	//TESTING ANIMATED SPRITES//

	Entity& animTest = entityManager.New( "animTest" );
	AddTransform( animTest );
	AddAnimatedSprite( animTest );

	Transform& animTransform = GetTransform( animTest );
	animTransform.translation.x = -300;
	animTransform.translation.y = -200;
	animTransform.scale.x = 100;
	animTransform.scale.y = 100;

	Animation2D testAnimation( "test" );
	Frame one( EngineDuration( std::chrono::seconds( 1 ) ) );
	Frame two( EngineDuration( std::chrono::seconds( 1 ) ) );
	Frame three( EngineDuration( std::chrono::seconds( 1 ) ) );

	one.TextureDimensions.right = 0.1f;
	one.TextureDimensions.bottom = 0.1f;
	one.Texture = assets.GetTexture( UVTEST );

	two.TextureDimensions.left = 0.1f;
	two.TextureDimensions.right = 0.2f;
	two.TextureDimensions.bottom = 0.1f;
	two.Texture = assets.GetTexture( UVTEST );

	three.TextureDimensions.left = 0.2f;
	three.TextureDimensions.right = 0.3f;
	three.TextureDimensions.bottom = 0.1f; 
	three.Texture = assets.GetTexture( UVTEST );

	testAnimation.AddFrame( one );
	testAnimation.AddFrame( two );
	testAnimation.AddFrame( three );

	GetAnimatedSprite( animTest ).AddAnimation( testAnimation );
	GetAnimatedSprite( animTest ).PlayAnimation( "test", Animation2D::PlaybackMode::LOOP );

	//Always set state and report
	loaded = true;
}

void TestScene::OnLost( void )
{
	if( !lost )
	{
		log.Message( "Lost, releasing resources", true );
		if( cubeBuffer != NULL )
		{
			log.Message( "Releasing cube buffer", true );
			cubeBuffer->Release();
			cubeBuffer = NULL;
		}

		lost = true;
	}
}

void TestScene::OnRecover( void )
{
	if( lost )
	{
		log.Message( "Reloading", true );
		//graphics.SetRenderStates();
		Vertex3dTx verts[] = { 
		// Front Face (1-2-3-4)
		{ -1.0f, 1.0f, -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, -1.0f, 1.0f, 0.0f }, { -1.0f, -1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, -1.0f, -1.0f, 1.0f, 1.0f },
		// Right Face (2-6-4-8)
		{ 1.0f, 1.0f, -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f, 0.0f }, { 1.0f, -1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, -1.0f, 1.0f, 1.0f, 1.0f },
		// Top Face (5-6-1-2)
		{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f, 0.0f }, { -1.0f, 1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, -1.0f, 1.0f, 1.0f },
		// Back Face (6-5-8-7)
		{ 1.0f, 1.0f, 1.0f, 0.0f, 0.0f }, { -1.0f, 1.0f, 1.0f, 1.0f, 0.0f }, { 1.0f, -1.0f, 1.0f, 0.0f, 1.0f }, { -1.0f, -1.0f, 1.0f, 1.0f, 1.0f },
		// Left Face (5-1-7-3)
		{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f }, { -1.0f, 1.0f, -1.0f, 1.0f, 0.0f }, { -1.0f, -1.0f, 1.0f, 0.0f, 1.0f }, { -1.0f, -1.0f, -1.0f, 1.0f, 1.0f },
		// Bottom Face (3-4-7-8)
		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f }, { 1.0f, -1.0f, -1.0f, 1.0f, 0.0f }, { -1.0f, -1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, -1.0f, 1.0f, 1.0f, 1.0f }
		};

		graphics.ErrorCheck
		( 
			graphics.Device()->CreateVertexBuffer( 24*sizeof(Vertex3dTx), D3DUSAGE_WRITEONLY, Vertex3dTx::format, D3DPOOL_DEFAULT, &cubeBuffer, NULL ), 
			"cubeBuffer creation failed"
		);

		VOID* pVoid;
		graphics.ErrorCheck( cubeBuffer->Lock( 0, 0, (void**)&pVoid, 0 ), "SplashScreen: Error locking cube buffer" );
		memcpy( pVoid, verts, sizeof(verts) );
		graphics.ErrorCheck( cubeBuffer->Unlock(), "SplashScreen: Error unlocking cube buffer" );

		lost = false;
	}
}

void TestScene::Unload( void )
{
	if( cubeBuffer != NULL )
	{
		cubeBuffer->Release();
		cubeBuffer = NULL;
	}
	if( cubeTex != NULL )
	{
		cubeTex->Release();
		cubeTex = NULL;
	}
	if( font != NULL )
	{
		font->Release();
		font = NULL;
	}

	bool Playing = true;
	
	if( channel )
	{
		channel->isPlaying( &Playing );
		if( Playing ){ sound.ErrorCheck( channel->setLoopCount( 0 ), "Main: Stopping sound" ); }
	}

	//Always set state and report
	loaded = false;
}

void TestScene::Update( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), UPDATE );

	//log.Message( std::to_string( system.time.DeltaTime().count() ).c_str() );

	// transforms.GetTransformComponent( entityManager["qMark"].ID ).localRotation.z -= 0.1;
	GetTransform( entityManager["qMark"] ).localRotation.z -= ( 0.01 );

	/*if( test1.IsAnyPressed() )
	{
		log.Message( "Logical Device: Any Pressed!", true );
	}*/

	if( test1.IsChordPressed() )
	{
		GetSprite( entityManager["qMark"] ).TextureDimensions.right -= 0.5f;
		GetSprite( entityManager["qMark"] ).TextureDimensions.bottom -= 0.5f;

		// bool Playing = false;
		// bool Paused = false;
		// if( channel )
		// {
		// 	channel->isPlaying( &Playing );
		// 	channel->getPaused( &Paused );
		// 	//if( Playing ){ sound.ErrorCheck( channel->setLoopCount( 0 ), "Main: Stopping sound" ); }
		// 	//else{ sound.ErrorCheck( channel->setLoopCount( 1 ), "Main: Starting sound" ); }

		// 	if( Playing && !Paused )
		// 	{
		// 		sound.ErrorCheck( channel->setPaused( true ), "Pausing" );
		// 	}
		// 	else
		// 	{
		// 		sound.ErrorCheck( channel->setPaused( false ), "Un-pausing" );
		// 	}
		// }
	}
}

void TestScene::PreRender( void )
{
	graphics.Device()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 150, 150, 150 ), 1.0f, 0 );

	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_LIGHTING, false ), "Setting Lighting State Failed" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW ), "Setting Culling Mode Failed" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE ), "Enabling Depth Testing" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_COLORVERTEX, TRUE ), "Enabling Vertex Colours" );

	graphics.Device()->BeginScene();

	engine.UpdateProcesses( system.time.DeltaTime(), PRE_RENDER );
}

void TestScene::Render( void )
{
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_LIGHTING, false ), "Setting Lighting False" );

	graphics.Device()->SetFVF(Vertex3dTx::format);

	graphics.Device()->SetTransform( D3DTS_WORLD, &matWorld );
	graphics.Device()->SetTransform( D3DTS_VIEW, &matView );
	graphics.Device()->SetTransform( D3DTS_PROJECTION, &matProj );

	graphics.ErrorCheck( graphics.Device()->SetStreamSource( 0, cubeBuffer, 0, sizeof(Vertex3dTx) ), "Setting stream source" );
	graphics.Device()->SetTexture( 0, cubeTex );

	//graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 ), "Drawing cube" );
	//graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 4, 2 ), "Drawing cube" );
	//graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 8, 2 ), "Drawing cube" );
	//graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 12, 2 ), "Drawing cube" );
	//graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 16, 2 ), "Drawing cube" );
	//graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 20, 2 ), "Drawing cube" );

	engine.UpdateProcesses( system.time.DeltaTime(), RENDER );
}

void TestScene::PostRender( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), POST_RENDER );

	graphics.Device()->EndScene();

	graphics.Device()->Present( NULL, NULL, NULL, NULL );
}