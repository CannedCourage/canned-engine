//Implementation of SceneManager

#include "SceneManager\SceneManager.h"

int SceneManager::Initialise( const HINSTANCE hInstance, const LPSTR lpCmdLine, const int nCmdShow )
{
	currentScene = NULL;
	nextScene = NULL;
	currentSceneFinished = false;
	nextSceneReady = false;
	update = false;
	render = false;

	int result = window.Create( hInstance, lpCmdLine, nCmdShow );

	graphics.Initialise();

	//TO DO: determine default starting scene then initialise
	//Temp: Use SplashScreen
	currentScene = new SplashScreen();
	//currentScene->Load();

	currentSceneFinished = false;
	nextSceneReady = false;

	return result;
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
	graphics.DoChecks();
	//TODO: Create game object manager to handle Lost and Recover events, etc.
	if( graphics.IsDeviceLost() )	//Device is lost
	{
		OnLost();	//Destroy existing resources
		log.Message( "Device Lost", true );
		if( graphics.Reset() )	//Reset() will return true if the device was successfully 'found'
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

//If the SceneManager is shutting down, that means end of program, time to do cleanup
void SceneManager::Shutdown( void )
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

	graphics.CleanUp();

	window.Destroy();
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