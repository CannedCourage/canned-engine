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
	TRACE( "Buffers Swapping" );

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

//TODO: Load "nextScene" asynchronously
//See the following links for C++ 11 async libraries
// https://www.classes.cs.uchicago.edu/archive/2013/spring/12300-1/labs/lab6/
// http://en.cppreference.com/w/cpp/thread/thread/thread
// http://en.cppreference.com/w/cpp/thread/async
// http://en.cppreference.com/w/cpp/thread/future
// http://en.cppreference.com/w/cpp/thread/future/wait_for
// https://solarianprogrammer.com/2012/10/17/cpp-11-async-tutorial/
void SceneManager::QueueScene( const std::string& SceneName )
{
	nextScene.reset();

	nextScene = GetScene( SceneName );

	nextScene->Load();
}