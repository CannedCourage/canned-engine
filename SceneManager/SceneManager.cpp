//Implementation of SceneManager

#include "System/System.h"
#include "SceneManager/SceneManager.h"
#include "SceneManager/Scenes.h"
#include "SceneManager/IScene.h"

SceneManager::SceneManager( System &s ) : system( s ), log( "SceneManager" )
{
	currentScene = NULL;
	nextScene = NULL;
	currentSceneFinished = false;
	nextSceneReady = false;
	update = true;
	render = true;

	//TO DO: determine default starting scene then initialise
	//Temp: Use SplashScreen
	currentScene = new SplashScreen( system );
	//currentScene->Load();

	currentSceneFinished = false;
	nextSceneReady = false;
}

bool SceneManager::Update( void )
{
	if( !update ){ return false; }

	if( currentSceneFinished && nextSceneReady )
	{
		SwapSceneBuffers();
	}
	
	if( currentScene != NULL )
	{
		if( currentScene->IsLoaded() )
		{
			currentScene->Update();
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool SceneManager::FixedUpdate( void )
{
	if( !update ){ return false; }
	return true;
}

void SceneManager::Render()
{
	if( !render ){ return; }

	//TODO: Create game object manager to handle Lost and Recover events, etc.
	if( system.graphics.IsDeviceLost() )	//Device is lost
	{
		log.Message( "Device Lost", true );
		system.graphics.Refresh();
	}
	else	//Draw
	{
		if( currentScene != NULL )
		{
			if( currentScene->IsLoaded() )
			{
				currentScene->Render();
			}
			else
			{
				currentScene->Load();
			}
		}
	}
}

void SceneManager::OnLost( void )
{
	if( currentScene != NULL )
	{
		if( currentScene->IsLoaded() )
		{
			currentScene->OnLost();
		}
	}
	if( nextScene != NULL )
	{
		if( nextScene->IsLoaded() )
		{
			nextScene->OnLost();
		}
	}
}

void SceneManager::OnRecover( void )
{
	if( currentScene != NULL )
	{
		if( currentScene->IsLoaded() )
		{
			currentScene->OnRecover();
		}
	}
	if( nextScene != NULL )
	{
		if( nextScene->IsLoaded() )
		{
			nextScene->OnRecover();
		}
	}
}

SceneManager::~SceneManager( void )
{
	if( nextScene != NULL )
	{
		nextScene->Unload();
	}

	if( currentScene != NULL )
	{
		currentScene->Unload();
	}
}