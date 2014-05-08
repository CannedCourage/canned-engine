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
	engine.UpdateProcesses( system.time.deltaTimeS(), PRE_UPDATE );
}

void IScene::Update( void )
{
	MainLoop(); //Keep for now, remove eventually

	engine.UpdateProcesses( system.time.deltaTimeS(), UPDATE );
}

void IScene::FixedUpdate( void )
{
	engine.UpdateProcesses( system.time.deltaTimeS(), FIXED_UPDATE );
}

void IScene::PostUpdate( void )
{
	engine.UpdateProcesses( system.time.deltaTimeS(), POST_UPDATE );
}

void IScene::PreRender( void )
{
	engine.UpdateProcesses( system.time.deltaTimeS(), PRE_RENDER );
}

void IScene::Render( void )
{
	RenderMain(); //Keep for now, remove eventually

	engine.UpdateProcesses( system.time.deltaTimeS(), RENDER );
}

void IScene::PostRender( void )
{
	engine.UpdateProcesses( system.time.deltaTimeS(), POST_RENDER );
}

inline const bool IScene::IsLoaded( void ) const
{
	return loaded;
}