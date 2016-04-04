//Base scene functionality
#include "SceneManager/IScene.h"
#include "System/System.h"

IScene::IScene( System &sys, const std::string& N ) : 	system( sys ),
														manager( sys.sceneManager ),
														settings( sys.settings ),
														graphics( sys.graphics ),
														input( sys.input ),
														sound( sys.sound ),
														assets( sys.assets ),
														log( N ),
														Name( N )
{
}

IScene::~IScene( void )
{
}

void IScene::PreUpdate( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), Priority::PRE_UPDATE );
}

void IScene::Update( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), Priority::UPDATE );
}

void IScene::FixedUpdate( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), Priority::FIXED_UPDATE );
}

void IScene::PostUpdate( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), Priority::POST_UPDATE );
}

void IScene::PreRender( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), Priority::PRE_RENDER );
}

void IScene::Render( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), Priority::RENDER );
}

void IScene::PostRender( void )
{
	engine.UpdateProcesses( system.time.DeltaTime(), Priority::POST_RENDER );
}

inline const bool IScene::IsLoaded( void ) const
{
	return loaded;
}