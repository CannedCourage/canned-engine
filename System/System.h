#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <Windows.h>

#include "Logging/Log.h"
#include "SceneManager/SceneManager.h"
#include "Engine/Settings.h"
#include "Window/WindowMS.h"
#include "Graphics/Graphics.h"
#include "Engine/Time.h"
#include "Input/Input.h"

class System
{
private:

protected:

	Log log;
public:

	Time time;

	//Engine
	Settings settings;
	SceneManager sceneManager;
	WindowMS window;
	Graphics graphics;
	Input input;
	//Sound sound;

	System( void );
	~System( void );

	int Initialise( const HINSTANCE hInstance, const LPSTR lpCmdLine, const int nCmdShow );
	int Run( void );
	int GameLoop( void );
	void Shutdown( void );
	void Quit( void );

	LRESULT CALLBACK MessageHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
};

inline void System::Quit( void )
{
	PostMessage( window.getHandle(), WM_CLOSE, 0, 0 );
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

#endif //_SYSTEM_H_