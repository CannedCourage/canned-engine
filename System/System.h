#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <Windows.h>

#include "Logging\ILoggable.h"
#include "Engine\Modules.h"

//Engine

class System : ILoggable
{
private:

protected:

public:

	//Engine
	SceneManager sceneManager;
	DefaultSettings settings;
	Window window;
	Graphics graphics;
	//Input input;
	//Sound sound;

	System( void );
	~System( void );

	int Initialise( const HINSTANCE hInstance, const LPSTR lpCmdLine, const int nCmdShow );
	int Run( void );
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