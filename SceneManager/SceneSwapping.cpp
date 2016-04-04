#include "SceneManager/SceneManager.h"
#include "SceneManager/IScene.h"

#include <algorithm>

void SceneManager::SwapSceneBuffers( void )
{
	log.Message( "Buffers Swapping" );

	currentScene = nextScene;
	nextScene = NULL;
}

void SceneManager::ChangeScene( const std::string& SceneName )
{
	if( currentScene != NULL )
	{
		currentScene->End();
		currentScene->Unload();
	}

	SceneList& sceneList = GetSceneList();

	auto SceneFactory = sceneList.at( SceneName ); //'at' method is bounds-checked
	
	currentScene = SceneFactory( system );

	currentScene->Load();
}

void SceneManager::QueueScene( const std::string& SceneName )
{
	//TODO: Add code to destroy existing nextScene, if not null

	SceneList& sceneList = GetSceneList();

	auto SceneFactory = sceneList.at( SceneName ); //'at' method is bounds-checked
	
	nextScene = SceneFactory( system );

	nextScene->Load();
}