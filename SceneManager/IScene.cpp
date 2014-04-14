//Base scene functionality
#include "SceneManager/IScene.h"
#include "System/System.h"

IScene::IScene( System &sys, const char* n, sceneStates s ) : 	system( sys ),
																manager( sys.sceneManager ),
																settings( sys.settings ),
																graphics( sys.graphics ),
																input( sys.input ),
																sound( sys.sound ),
																assets( sys.assets ),
																meshes( sys.graphics ),
																log( n ),
																name( n ),
																state( s ),
																loaded( false ),
																lost( false )
{
}

IScene::~IScene( void )
{
}

void IScene::Update( void )
{
	if( !loaded )
	{
		Load();
	}

	switch( state )
	{
	case init:
		FadeIn();
		break;

	case update:
		MainLoop();
		break;

	case out:
		FadeOut();
		break;
	}
}

void IScene::Render( void )
{
	if( lost )
		return;

	if( !loaded )
	{
		Load();
	}

	switch( state )
	{
	case init:
		RenderIn();
		break;

	case update:
		RenderMain();
		break;

	case out:
		RenderOut();
		break;
	}
	//Update state here so that the correct and corresponding render method is called, even if the state changes in the update method
	state = nextState;
}

inline const bool IScene::IsLoaded( void ) const
{
	return loaded;
}

inline void IScene::SetState( sceneStates s )
{
	nextState = s;
}