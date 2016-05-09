#include "SceneManager/IScene.h"
#include "System/System.h"

IScene::IScene( System& Sys, const std::string& N ) : 	system( Sys ),
														manager( Sys.sceneManager ),
														settings( Sys.settings ),
														graphics( Sys.graphics ),
														input( Sys.input ),
														sound( Sys.sound ),
														assets( Sys.assets ),
														log( N ),
														Name( N )
{
}

IScene::~IScene( void )
{
}

void IScene::PreUpdate( void )
{
	engine.UpdateProcesses( system.time.DeltaTimeScaled(), Priority::PRE_UPDATE );
}

void IScene::Update( void )
{
	engine.UpdateProcesses( system.time.DeltaTimeScaled(), Priority::UPDATE );
}

void IScene::FixedUpdate( void )
{
	engine.UpdateProcesses( system.time.DeltaTimeScaled(), Priority::FIXED_UPDATE );
}

void IScene::PostUpdate( void )
{
	engine.UpdateProcesses( system.time.DeltaTimeScaled(), Priority::POST_UPDATE );
}

void IScene::PreRender( void )
{
	engine.UpdateProcesses( system.time.DeltaTimeScaled(), Priority::PRE_RENDER );
}

void IScene::Render( void )
{
	engine.UpdateProcesses( system.time.DeltaTimeScaled(), Priority::RENDER );
}

void IScene::PostRender( void )
{
	engine.UpdateProcesses( system.time.DeltaTimeScaled(), Priority::POST_RENDER );
}

inline const bool IScene::IsLoaded( void ) const
{
	return loaded;
}