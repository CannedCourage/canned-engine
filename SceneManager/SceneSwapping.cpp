#include "SceneManager/SceneManager.h"
#include "SceneManager/IScene.h"

#include <algorithm>

std::unique_ptr<IScene> SceneManager::GetScene( const std::string& SceneName )
{
	//Method "at" is bounds-checked, will throw if no key called "SceneName" exists
	return GetSceneList().at( SceneName )( system );
}

void SceneManager::SwapSceneBuffers( void )
{
	log( "Buffers Swapping" );

	currentScene = std::move( nextScene );

	nextScene.reset();
}

void SceneManager::ChangeScene( const std::string& SceneName )
{
	if( currentScene )
	{
		currentScene->End();
		currentScene->Unload();
	}

	currentScene = GetScene( SceneName );

	currentScene->Load();
}

void SceneManager::QueueScene( const std::string& SceneName )
{
	nextScene.reset();

	nextScene = GetScene( SceneName );

	nextScene->Load();
}