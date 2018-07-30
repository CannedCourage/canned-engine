#include "Game/Scenes/TestScene.h"
#include "System/System.h"

bool regTestScene = RegisterScene<TestScene>( "TestScene" );

//Globals for scene variables?
//Would multiple instances of this scene share global variables?

TestScene::TestScene( System &s ) : IScene( s, "TestScene" ),
									keys( input ),
									logiMouse( input ),
									player1( input ),
									test1( input ),
									transforms(),
									//meshes( s.graphics ),
									//sprites( s.graphics, transforms ),
									//animatedSprites( s.graphics, transforms ),
									AddTransform( transforms ),
									GetTransform( transforms )
									//AddSprite( sprites ),
									//GetSprite( sprites ),
									//AddAnimatedSprite( animatedSprites ),
									//GetAnimatedSprite( animatedSprites )
{
	test1.AddInput( std::bind( &Keyboard::IsPressed, &keys, Keyboard::Keys::LEFT_CONTROL ) );

	test1.AddInput( std::bind( &Keyboard::WentUp, &keys, Keyboard::Keys::LEFT_ALT ) );

	engine.AddProcess( &transforms, Priority::UPDATE );
	//engine.AddProcess( &meshes, Priority::RENDER );
	//engine.AddProcess( &sprites, Priority::RENDER );
	//engine.AddProcess( &animatedSprites, Priority::RENDER );
}

TestScene::~TestScene( void )
{
	Unload();
}

void TestScene::Load( void )
{
	TRACE( "Loading" );

	sound.LoadSoundSample( "w:/game/sounds/drumloop.wav", "drumloop" );
	
	//graphics.LoadTexture( "w:/game/textures/qMark.bmp", "MarioQuestionBlockTexture" );
	//graphics.LoadTexture( "w:/game/textures/uvtemplate.bmp", "uvtest" );
	//graphics.LoadTexture( "w:/game/textures/transparency_test.png", "fucknope" );

	//Always set state and report
	State = SceneState::INIT;
	loaded = true;
}

void TestScene::Start( void )
{
	TRACE( "TestScene Start method!" );

	Entity& qMark = entityManager.New( "qMark" );

	//TODO: Need to add delete* methods to remove components
	AddTransform( qMark );
	//AddSprite( qMark, "MarioQuestionBlockTexture");

	GetTransform( qMark ).translation.z = 5;

	GetTransform( qMark ).scale.x = 512;
	GetTransform( qMark ).scale.y = 512;

	//GetSprite( qMark ).TextureDimensions.right = 2.0f;
	//GetSprite( qMark ).TextureDimensions.bottom = 2.0f;

	//TESTING ANIMATED SPRITES//

	Entity& animTest = entityManager.New( "animTest" );
	
	AddTransform( animTest );
	//AddAnimatedSprite( animTest );

	Transform& animTransform = GetTransform( animTest );
	animTransform.translation.x = -300;
	animTransform.translation.y = -200;
	animTransform.translation.z = 6;
	animTransform.scale.x = 100;
	animTransform.scale.y = 100;

	//Animation2D testAnimation( "test" );
	/*
	Frame one = animatedSprites.NewFrame( EngineDuration( std::chrono::seconds( 1 ) ), "uvtest" );
	Frame two = animatedSprites.NewFrame( EngineDuration( std::chrono::seconds( 1 ) ), "uvtest" );
	Frame three = animatedSprites.NewFrame( EngineDuration( std::chrono::seconds( 1 ) ), "uvtest" );

	one.TextureDimensions.right = 0.1f;
	one.TextureDimensions.bottom = 0.1f;

	two.TextureDimensions.left = 0.1f;
	two.TextureDimensions.right = 0.2f;
	two.TextureDimensions.bottom = 0.1f;

	three.TextureDimensions.left = 0.2f;
	three.TextureDimensions.right = 0.3f;
	three.TextureDimensions.bottom = 0.1f; 

	testAnimation.AddFrame( one );
	testAnimation.AddFrame( two );
	testAnimation.AddFrame( three );

	GetAnimatedSprite( animTest ).AddAnimation( testAnimation );
	GetAnimatedSprite( animTest ).PlayAnimation( "test", Animation2D::PlaybackMode::LOOP );
	//*/

	//TRANSPARENCY TEST//
	Entity& tTest = entityManager.New( "tTest" );

	AddTransform( tTest );
	//AddSprite( tTest, "fucknope" );

	GetTransform( tTest ).scale.x = 79;
	GetTransform( tTest ).scale.y = 103;
	GetTransform( tTest ).translation.z = 4;

	State = SceneState::UPDATE;
}

void TestScene::OnLost( void )
{
	if( !lost )
	{
		TRACE( "Lost, releasing resources" );

		lost = true;
	}
}

void TestScene::OnRecover( void )
{
	if( lost )
	{
		TRACE( "Reloading" );

		lost = false;
	}
}

void TestScene::Unload( void )
{
	bool Playing = true;
	
	if( channel )
	{
		channel->isPlaying( &Playing );
		if( Playing ){ sound.ErrorCheck( channel->setLoopCount( 0 ), "Main: Stopping sound" ); }
	}

	//Always set state and report
	loaded = false;
}

//TODO: Add "Start" init method to iScene - This will hold init code separate to Load method, which could be called asynchronously
//TODO: Add "End" clean-up method(?)
void TestScene::Update( void )
{
	engine.UpdateProcesses( system.time.DeltaTimeActual(), Priority::UPDATE );

	//TODO: Add overload that accepts just string, that will then do the lookup for you?
	GetTransform( entityManager["qMark"] ).localRotation.z -= ( 0.01 );

	if( test1.IsChordPressed() )
	{
		TRACE("Chord Pressed");
		// bool Playing = false;
		// bool Paused = false;
		// if( channel )
		// {
		// 	channel->isPlaying( &Playing );
		// 	channel->getPaused( &Paused );
		// 	if( Playing ){ sound.ErrorCheck( channel->setLoopCount( 0 ), "Main: Stopping sound" ); }
		// 	else{ sound.ErrorCheck( channel->setLoopCount( 1 ), "Main: Starting sound" ); }
		// }
	}
}

void TestScene::PreRender( void )
{
	//graphics.Device()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 150, 150, 150 ), 1.0f, 0 );

	//graphics.Device()->BeginScene();

	//Should this be called before the scene begins?
	engine.UpdateProcesses( system.time.DeltaTimeActual(), Priority::PRE_RENDER );
}

void TestScene::PostRender( void )
{
	engine.UpdateProcesses( system.time.DeltaTimeActual(), Priority::POST_RENDER );

	//graphics.Device()->EndScene();

	//graphics.Device()->Present( NULL, NULL, NULL, NULL );
}

void TestScene::End( void )
{
}