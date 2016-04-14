//Class for managing scenes, updating, rendering and switching.

#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "Logging/Log.h"

#include <deque>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

class System;
class IScene;
class GUI;

//typedef std::function<IScene* ( System &s )> SceneFactoryFunction;
typedef std::function<std::unique_ptr<IScene> ( System& Sys )> SceneFactoryFunction;
typedef std::unordered_map<std::string, SceneFactoryFunction> SceneList;

class SceneManager
{
private:
protected:

	Log log;

	System& system;

	//IScene* currentScene = NULL;
	//IScene* nextScene = NULL;
	std::unique_ptr<IScene> currentScene;
	std::unique_ptr<IScene> nextScene;

	bool update = true;
	bool render = true;

	std::deque<GUI*> GUIStack;

	std::unique_ptr<IScene> GetScene( const std::string& SceneName );

	void SwapSceneBuffers( void );
public:

	static SceneList& GetSceneList( void );

	SceneManager( System& s );
	~SceneManager( void );

	void Init( void );
	void Shutdown( void );

	void Prepare( void );

	bool Update( void );
	bool FixedUpdate( void );
	void Render( void );

	void SetUpdate( const bool& b );
	void SetRender( const bool& b );
	void OnLost( void );
	void OnRecover( void );

	void ChangeScene( const std::string& SceneName );
	void QueueScene( const std::string& SceneName );

	//GUI
	void PushGUI( GUI* const newGUI );
	GUI* const PopGUI( void );
	GUI* const SwapToGUI( GUI* const newGUI );
	void ClearGUIStack( void );
};

inline void SceneManager::SetUpdate( const bool& b )
{
	update = b;
}

inline void SceneManager::SetRender( const bool& b )
{
	render = b;
}

template<class SceneType>
bool RegisterScene( std::string Name )
{
	SceneFactoryFunction func = []( System& Sys ){ return std::make_unique<SceneType>( Sys ); };

	return SceneManager::GetSceneList().emplace( std::move( Name ), std::move( func ) ).second;
}

#endif //_SCENEMANAGER_H_