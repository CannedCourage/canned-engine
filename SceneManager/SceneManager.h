//Class for managing scenes, updating, rendering and switching.

#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "Logging/Log.h"

#include <deque>
using std::deque;

class System;
class IScene;

class SceneManager
{
private:

	//const SceneManager& operator=( const SceneManager& src );
protected:

	Log log;

	System &system;

	IScene* currentScene;
	bool currentSceneFinished;
	IScene* nextScene;
	bool nextSceneReady;

	bool update;
	bool render;

	void SwapSceneBuffers( void );

	bool Update( void );
	void Render( void );
public:

	SceneManager( System &s );
	~SceneManager( void );

	void Run( void );

	void SetUpdate( const bool& b );
	void SetRender( const bool& b );
	void OnLost( void );
	void OnRecover( void );

	void ChangeScene( IScene* const newScene );
	void PreloadScene( IScene* const newScene );
	void ReportFinishedLoading( IScene* const scene );
	void ReportFinishedUnloading( IScene* const scene );
};

inline void SceneManager::SetUpdate( const bool& b )
{
	update = b;
}

inline void SceneManager::SetRender( const bool& b )
{
	render = b;
}

#endif //_SCENEMANAGER_H_