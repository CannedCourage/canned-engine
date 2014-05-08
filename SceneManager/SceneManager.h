//Class for managing scenes, updating, rendering and switching.

#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "Logging/Log.h"

#include <deque>
using std::deque;

#include <vector>

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

	std::vector<IScene*> sceneList;

	void SwapSceneBuffers( void );
public:

	SceneManager( System &s );
	~SceneManager( void );

	void Init( void );
	void Shutdown( void );

	bool Update( void );
	bool FixedUpdate( void );
	void Render( void );

	void SetUpdate( const bool& b );
	void SetRender( const bool& b );
	void OnLost( void );
	void OnRecover( void );

	void ChangeScene( unsigned int i );
	void PreloadScene( unsigned int i );
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