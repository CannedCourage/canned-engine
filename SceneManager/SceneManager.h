//Class for managing scenes, updating, rendering and switching.
//This is the top most level before WinMain

#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

//System
#include <Windows.h>
#include <assert.h>

//Engine
#include "Engine\Modules.h"
#include "Logging\ILoggable.h"
#include "SceneManager\IScene.h"
#include "GUI\GUI.h"

#include "SceneManager\Scenes.h"

#include <deque>
using std::deque;

class SceneManager : public ILoggable
{
private:

	SceneManager( void ){}
	~SceneManager( void ){}
	const SceneManager& operator=( const SceneManager& src );
protected:

	IScene* currentScene;
	bool currentSceneFinished;
	IScene* nextScene;
	bool nextSceneReady;
	deque<GUI*> GUIStack;

	bool update;
	bool render;

	static SceneManager* GetWkr( void )
	{
		SceneManager* pTmp	= NULL;
		try
		{
			static SceneManager tVar;
			pTmp = &tVar;
		}
		catch(...)
		{
			//s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP, "Couldn't get SceneManager instance.");
			assert(false);
			pTmp = NULL;
		}
		return pTmp;
	}

	void SwapSceneBuffers( void );

	bool Update( void );
	void Render( void );
public:

	//Engine
	DefaultSettings settings;
	Window window;
	Graphics graphics;
	Input input;
	Sound sound;

	int Initialise( const HINSTANCE hInstance, const LPSTR lpCmdLine, const int nCmdShow );
	int Run( void );
	void Shutdown( void );
	void Quit( void );
	void SetUpdate( const bool& b );
	void SetRender( const bool& b );
	void OnLost( void );
	void OnRecover( void );

	LRESULT CALLBACK MessageHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	//Static Functions
	static SceneManager* Get( void )
	{
		SceneManager* pTmp	= GetWkr();
		return pTmp;
	}

	void ChangeScene( IScene* const newScene );
	void PreloadScene( IScene* const newScene );
	void ReportFinishedLoading( IScene* const scene );
	void ReportFinishedUnloading( IScene* const scene );
	void PushGUI( GUI* const newGUI );
	GUI* const PopGUI( void );
	GUI* const SwapToGUI( GUI* const newGUI );
	void ClearGUIStack( void );
};

inline void SceneManager::Quit( void )
{
	PostMessage( window.getHandle(), WM_CLOSE, 0, 0 );
}

inline void SceneManager::SetUpdate( const bool& b )
{
	update = b;
}

inline void SceneManager::SetRender( const bool& b )
{
	render = b;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

#endif //_SCENEMANAGER_H_