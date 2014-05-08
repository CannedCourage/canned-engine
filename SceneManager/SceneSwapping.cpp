#include "SceneManager/SceneManager.h"
#include "SceneManager/IScene.h"

void SceneManager::SwapSceneBuffers( void )
{
	log.Message( "Buffers Swapping" );
	currentScene = nextScene;
	nextScene = NULL;
	currentSceneFinished = false;
	nextSceneReady = false;
}

void SceneManager::ChangeScene( unsigned int i )
{
	currentScene->Unload();
	
	currentScene = sceneList[i];
}

void SceneManager::PreloadScene( unsigned int i )
{
}