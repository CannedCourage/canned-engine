//Implementation of SceneManager

#include "System/System.h"
#include "SceneManager/SceneManager.h"
#include "SceneManager/Scenes.h"
#include "SceneManager/IScene.h"
#include "GUI/GUI.h"
#include "Game/Interfaces/MainMenuGUI.h"

SceneManager::SceneManager( System &s ) : system( s ), log( "SceneManager" )
{
	currentScene = NULL;
	nextScene = NULL;
	currentSceneFinished = false;
	nextSceneReady = false;
	update = true;
	render = true;
}

void SceneManager::Init( void )
{
	sceneList.push_back( new SplashScreen( system ) );
	sceneList.push_back( new TestScene( system ) );
	//TO DO: determine default starting scene then initialise
	//Temp: Use SplashScreen
	//currentScene = new SplashScreen( system );
	currentScene = sceneList[1];

	PushGUI( new MainMenu( system, "Main Menu" ) );
}

void SceneManager::Shutdown( void )
{
	log.Message( "Scene Shutdown Method", true );

	if( nextScene != NULL )
	{
		nextScene->Unload();
		nextScene = NULL;
	}

	if( currentScene != NULL )
	{
		currentScene->Unload();
		currentScene = NULL;
	}

	for( int i = 0; i < sceneList.size(); i++ )
	{
		if( sceneList[i] != NULL )
		{
			delete sceneList[i];
			sceneList[i] = NULL;
		}
	}

	ClearGUIStack();

	update = false;
	render = false;
}

bool SceneManager::Update( void )
{
	if( !update ){ return false; }
	
	if( currentScene != NULL )
	{
		if( currentScene->IsLoaded() )
		{
			currentScene->PreUpdate();
			currentScene->Update();
			currentScene->PostUpdate();
		}
	}
	else
	{
		return false;
	}

	if( !( GUIStack.empty() ) )
	{
		GUIStack.front()->Update();
	}

	return true;
}

bool SceneManager::FixedUpdate( void )
{
	if( !update ){ return false; }

	if( currentScene != NULL )
	{
		if( currentScene->IsLoaded() )
		{
			currentScene->FixedUpdate();
		}
	}

	return true;
}

void SceneManager::Render()
{
	if( !render ){ return; }

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
				currentScene->PreRender(); //BeginScene

				currentScene->Render();

				//Render GUI
				std::deque<GUI*>::reverse_iterator itr;

				for( itr = GUIStack.rbegin(); itr != GUIStack.rend(); itr++ )
				{
					( *itr )->Render();
				}

				currentScene->PostRender(); //EndScene
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
	Shutdown();
}