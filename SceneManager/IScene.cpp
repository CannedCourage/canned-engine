//Base scene functionality
#include "SceneManager/IScene.h"
#include "System/System.h"

IScene::IScene( System &sys, const char* n ) : 	system( sys ),
												manager( sys.sceneManager ),
												settings( sys.settings ),
												graphics( sys.graphics ),
												input( sys.input ),
												sound( sys.sound ),
												assets( sys.assets ),
												entityManager(),
												engine(),
												log( n ),
												name( n ),
												loaded( false ),
												lost( false )
{
}

IScene::~IScene( void )
{
}

void IScene::PreUpdate( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), PRE_UPDATE );
}

void IScene::Update( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), UPDATE );
}

void IScene::FixedUpdate( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), FIXED_UPDATE );
}

void IScene::PostUpdate( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), POST_UPDATE );
}

void IScene::PreRender( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), PRE_RENDER );
}

void IScene::Render( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), RENDER );
}

void IScene::PostRender( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), POST_RENDER );
}

inline const bool IScene::IsLoaded( void ) const
{
	return loaded;
}