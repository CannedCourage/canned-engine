//Implementation of SceneManager

#include "System\System.h"
#include "SceneManager\SceneManager.h"
#include "SceneManager\Scenes.h"
#include "SceneManager\IScene.h"
#include "GUI\GUI.h"

SceneManager::SceneManager( System &s ) : system( s )
{
	currentScene = NULL;
	nextScene = NULL;
	currentSceneFinished = false;
	nextSceneReady = false;
	update = false;
	render = false;

	//TO DO: determine default starting scene then initialise
	//Temp: Use SplashScreen
	currentScene = new SplashScreen( system );
	//currentScene->Load();

	currentSceneFinished = false;
	nextSceneReady = false;
}

void SceneManager::Run( void )
{
	if( update )
		{
			if( !Update() )
			{
				return;
			}
			else
			{
				if( render )
				{
					Render();
				}
			}
		}
}

bool SceneManager::Update( void )
{
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

	if( !GUIStack.empty() )
	GUIStack.front()->Update();

	return true;
}

void SceneManager::Render()
{
	system.graphics.DoChecks();
	//TODO: Create game object manager to handle Lost and Recover events, etc.
	if( system.graphics.IsDeviceLost() )	//Device is lost
	{
		OnLost();	//Destroy existing resources
		log.Message( "Device Lost", true );
		if( system.graphics.Reset() )	//Reset() will return true if the device was successfully 'found'
		{
			log.Message( "Recovering", true );
			OnRecover();	//Recreate resources that existed before the device was lost
		}
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

		deque<GUI*>::reverse_iterator itr;

		for( itr = GUIStack.rbegin(); itr != GUIStack.rend(); itr++ )
		{
			(*itr)->Render();
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
	ClearGUIStack();

	if( nextScene != NULL )
	{
		nextScene->Unload();
	}

	if( currentScene != NULL )
	{
		currentScene->Unload();
	}
}