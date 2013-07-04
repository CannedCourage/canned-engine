#include "SceneManager\SceneManager.h"

void SceneManager::ChangeScene( IScene* const newScene )
{
	if( currentScene != NULL )
	{
		currentScene->SetState( out );
	}

	PreloadScene( newScene );
}

void SceneManager::PreloadScene( IScene* const newScene )
{
	if( nextScene == NULL )
	{
		nextScene = newScene;

		nextScene->Load();
	}
	else
	{
		nextScene->Unload();

		delete nextScene;

		nextScene = newScene;

		nextScene->Load();
	}
}

void SceneManager::SwapSceneBuffers( void )
{
	currentScene = nextScene;
	nextScene = 0;
	currentSceneFinished = false;
	nextSceneReady = false;
}

void SceneManager::ReportFinishedLoading( IScene* const scene )
{
	if( scene == nextScene )
	{
		nextSceneReady = true;
	}
	if( scene == currentScene )
	{
		currentSceneFinished = false;
	}
}

void SceneManager::ReportFinishedUnloading( IScene* const scene )
{
	if( scene == nextScene )
	{
		nextScene = 0;
		nextSceneReady = false;
	}
	if( scene == currentScene )
	{
		currentScene = 0;
		currentSceneFinished = true;
	}
	delete scene;
}