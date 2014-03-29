#include "Game/Scenes/TestScene.h"
#include "System/System.h"

TestScene::TestScene( System &s ) : IScene( s, "TestScene" ), cubeBuffer( NULL ), cubeTex( NULL ), font( NULL ), buffer( s )
{
	D3DXMatrixTranslation( &matWorld, 0, 0, 0 );
	D3DXMatrixLookAtLH( &matView, &(D3DXVECTOR3( 3.0f, 3.0f, -3.0f )), &(D3DXVECTOR3( 0.0f, 0.0f, 0.0f )), &(D3DXVECTOR3( 0.0f, 1.0f, 0.0f )) );
	D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 90.0f ), 
		(float)settings.GetInteger( "display/xResolution" )/(float)settings.GetInteger( "display/yResolution" ), 
		1.0f, 100.0f );

	FontPosition.top = 0;
	FontPosition.left = 0;
	FontPosition.right = settings.GetInteger( "display/xResolution" );
	FontPosition.bottom = settings.GetInteger( "display/yResolution" );

	input.Register( &keys );
	input.Register( &logiMouse );
	input.Register( &player1 );
}

TestScene::~TestScene( void )
{
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
		D3DXCreateTextureFromFile( graphics.Device(), TEXT( "C:/Users/Scott/Programming/Projects/GitHub/canned-engine/Game/Textures/qMark.bmp" ), &cubeTex ), 
		TEXT( "Failed creating texture" ) 
	);

	D3DXFONT_DESC FontDesc = { 24, 0, 400, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_PITCH, TEXT( "Arial" ) };
	D3DXCreateFontIndirect( graphics.Device(), &FontDesc, &font );

	//Meshes
	assets.LoadMesh( "tiger.x" );
	Entity tiger = entityManager.New();
	meshes.AddMeshComponent( tiger, assets.GetMesh( 0 ) );

	//Always set state and report
	loaded = true;
	system.sceneManager.ReportFinishedLoading( this );
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
		if( font != NULL )
		{
			graphics.ErrorCheck( font->OnLostDevice(), "Error Font, lost device" );
		
		}

		lost = true;
	}
}

void TestScene::OnRecover( void )
{
	if( lost )
	{
		log.Message( "ReLoading", true );
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
			TEXT( "cubeBuffer creation failed" ) 
		);

		VOID* pVoid;
		graphics.ErrorCheck( cubeBuffer->Lock( 0, 0, (void**)&pVoid, 0 ), TEXT( "SplashScreen: Error locking cube buffer" ) );
		memcpy( pVoid, verts, sizeof(verts) );
		graphics.ErrorCheck( cubeBuffer->Unlock(), TEXT( "SplashScreen: Error unlocking cube buffer" ) );

		//D3DXFONT_DESC FontDesc = { 24, 0, 400, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_PITCH, TEXT( "Arial" ) };
		//D3DXCreateFontIndirect( graphics.Device(), &FontDesc, &font );
		graphics.ErrorCheck( font->OnResetDevice(), TEXT( "Error recovering font" ) );

		lost = false;
	}
}

void TestScene::Unload( void )
{
	log.Message( "Unloading", true );
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
	//Always set state and report
	loaded = false;
	system.sceneManager.ReportFinishedUnloading( this );
}

void TestScene::FadeIn( void )
{
	SetState( update );
}

void TestScene::MainLoop( void )
{
	if( player1.WentUp( XboxController::Button::A ) )
	{
		log.Message( "A button detected!", true );
	}

	if( keys.WentDown( Keyboard::Keys::LEFT_CONTROL ) )
	{
		log.Message( "A key is going down!", true );
	}

	if( keys.WentUp( Keyboard::Keys::LEFT_CONTROL ) )
	{
		log.Message( "A key is going up!", true );
	}

	if( logiMouse.IsPressed( Mouse::Button::LEFT ) )
	{
		log.Message( "Left mouse button is pressed", true );
	}

	if( logiMouse.WentDown( Mouse::Button::LEFT ) )
	{
		log.Message( "Left mouse button is going down", true );
	}

	if( logiMouse.WentUp( Mouse::Button::LEFT ) )
	{
		log.Message( "Left mouse button is going up", true );
	}
}

void TestScene::FadeOut( void )
{
	Unload();
}

void TestScene::RenderIn( void )
{
}

void TestScene::RenderMain( void )
{
	graphics.Device()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 150, 150, 150 ), 1.0f, 0 );

	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_LIGHTING, false ), "Setting Lighting State Failed" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW ), "Setting Culling Mode Failed" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE ), "Enabling Depth Testing" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_COLORVERTEX, TRUE ), "Enabling Vertex Colours" );

	graphics.Device()->BeginScene();

	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_LIGHTING, false ), "Setting Lighting False" );

	graphics.Device()->SetFVF(Vertex3dTx::format);

	graphics.Device()->SetTransform( D3DTS_WORLD, &matWorld );
	graphics.Device()->SetTransform( D3DTS_VIEW, &matView );
	graphics.Device()->SetTransform( D3DTS_PROJECTION, &matProj );

	graphics.ErrorCheck( graphics.Device()->SetStreamSource( 0, cubeBuffer, 0, sizeof(Vertex3dTx) ), "Setting stream source" );
	graphics.Device()->SetTexture( 0, cubeTex );

	graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 ), "Drawing cube" );
	graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 4, 2 ), "Drawing cube" );
	graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 8, 2 ), "Drawing cube" );
	graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 12, 2 ), "Drawing cube" );
	graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 16, 2 ), "Drawing cube" );
	graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 20, 2 ), "Drawing cube" );

	font->DrawText( NULL, "Text Sample Using D3DXFont", -1, &FontPosition, DT_CENTER | DT_BOTTOM, 0xffffffff );

	//Mesh manager stuff here
	float placeholderDeltaT = 0.0f;
	meshes.Render( placeholderDeltaT );

	graphics.Device()->EndScene();

	graphics.Device()->Present( NULL, NULL, NULL, NULL );
}

void TestScene::RenderOut( void )
{
}

//graphics.Device()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
//graphics.Device()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);	//Use the diffuse component of the vertex
//graphics.Device()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
//graphics.Device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
//graphics.Device()->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
//graphics.Device()->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);