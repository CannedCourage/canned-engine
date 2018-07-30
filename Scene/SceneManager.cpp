#include "System/System.h"
#include "Scene/SceneManager.h"
#include "Scene/IScene.h"
#include "GUI/GUI.h"
#include "Game/Interfaces/MainMenuGUI.h"

SceneList& SceneManager::GetSceneList( void )
{
	static SceneList scenes;

	return scenes;
}

SceneManager::SceneManager( System &s ) : system( s ), log( "SceneManager" )
{
}

void SceneManager::Init( void )
{
	TRACE( "SceneManager initialisation" );
	
	//TODO: Determine default starting scene then initialise
	//With new SceneList implementation, this can be read from a settings file as a string
	ChangeScene( "TestScene" );

	PushGUI( new MainMenu( "Main Menu" ) );
}

void SceneManager::Prepare( void )
{
	if( currentScene && nextScene )
	{
		if( currentScene->State == SceneState::END )
		{
			if( nextScene->IsLoaded() )
			{
				SwapSceneBuffers();
			}
		}
	}

}

bool SceneManager::Update( void )
{
	if( !update ){ return false; }
	
	if( currentScene )
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

	if( currentScene )
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
	/*
	if( !render ){ return; }

	if( system.graphics.IsDeviceLost() ) //Device is lost
	{
		TRACE( "Device Lost" );

		system.graphics.Refresh();
	}
	else	//Draw
	{
		if( currentScene )
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
	//*/
}

void SceneManager::OnLost( void )
{
	if( currentScene )
	{
		if( currentScene->IsLoaded() )
		{
			currentScene->OnLost();
		}
	}

	if( nextScene )
	{
		if( nextScene->IsLoaded() )
		{
			nextScene->OnLost();
		}
	}
}

void SceneManager::OnRecover( void )
{
	if( currentScene )
	{
		if( currentScene->IsLoaded() )
		{
			currentScene->OnRecover();
		}
	}

	if( nextScene )
	{
		if( nextScene->IsLoaded() )
		{
			nextScene->OnRecover();
		}
	}
}

void SceneManager::Shutdown( void )
{
	TRACE( "Scene Shutdown Method" );

	if( nextScene )
	{
		nextScene->Unload();
		nextScene.reset();
	}

	if( currentScene )
	{
		currentScene->Unload();
		currentScene.reset();
	}

	ClearGUIStack();

	update = false;
	render = false;
}

SceneManager::~SceneManager( void )
{
	Shutdown();
}