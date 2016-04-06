#include "System/System.h"
#include "SceneManager/SceneManager.h"
#include "SceneManager/IScene.h"
#include "GUI/GUI.h"
#include "Game/Interfaces/MainMenuGUI.h"

SceneList& SceneManager::GetSceneList( void )
{
	static SceneList* scenes = new SceneList();

	return ( *scenes );
}

SceneManager::SceneManager( System &s ) : system( s ), log( "SceneManager" )
{
}

void SceneManager::Init( void )
{
	log.Message( "SceneManager initialisation" );
	
	//TODO: Determine default starting scene then initialise
	//With new SceneList implementation, this can be read from a settings file as a string
	ChangeScene( "TestScene" );

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

	ClearGUIStack();

	update = false;
	render = false;
}

void SceneManager::Prepare( void )
{
	if( currentScene != NULL )
	{
		if( currentScene->State == SceneState::END )
		{
			if( nextScene != NULL )
			{
				if( nextScene->IsLoaded() )
				{
					SwapSceneBuffers();
				}
			}
		}
	}

}

bool SceneManager::Update( void )
{
	if( !update ){ return false; }
	
	if( currentScene != NULL )
	{
		if( currentScene->IsLoaded() )
		{
			if( currentScene->State == SceneState::INIT )
			{
				currentScene->Start();
			}

			if( currentScene->State == SceneState::UPDATE )
			{
				currentScene->PreUpdate();
				currentScene->Update();
				currentScene->PostUpdate();
			}

			if( currentScene->State == SceneState::CLEANUP )
			{
				currentScene->End();
			}
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