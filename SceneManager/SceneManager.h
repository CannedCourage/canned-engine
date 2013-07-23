//Class for managing scenes, updating, rendering and switching.

#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

//Engine
#include "Logging\ILoggable.h"

#include <deque>
using std::deque;

class System;
class IScene;
class GUI;

class SceneManager : public ILoggable
{
private:

	//const SceneManager& operator=( const SceneManager& src );
protected:

	System &system;

	IScene* currentScene;
	bool currentSceneFinished;
	IScene* nextScene;
	bool nextSceneReady;
	deque<GUI*> GUIStack;

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

#endif //_SCENEMANAGER_H_