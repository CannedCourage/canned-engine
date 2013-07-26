#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <Windows.h>
#include "Logging/Log.h"

class System;
class Settings;

class Window
{
private:
protected:

	Log log;

	Settings &settings;

	HWND hwnd;				//Handle
	WNDCLASSEX windowClass;	//A structure that stores information used to create the window
	HINSTANCE hInstance;
	HWND graphicsDialog, colourDialog, depthDialog, sampleDialog, displayDialog, aspectDialog;
	HWND goButton;

	int x, y, width, height;
	bool maximised;
public:

	Window( System &s );
	~Window( void );

	HWND const getHandle( void ) const;

	int Create( const HINSTANCE hInstance, const LPSTR lpCmdLine, const int nCmdShow );
	void Destroy( void );
	int Recreate( void );

	void AddButton( void );
	void AdapterDialog( void );
	void AddAdapterOption( const char* adapter );
	void AddDisplayOption( const char* adapter );
	void ClearDisplayOptions( void );
	void DestroyDialogs( void );

	void Update( void );

	void SetPosition( const int& X, const int& Y );
	void SetSize( const int& Width, const int& Height );
	void Cursor( const bool& show );
	void MoveCursor( const int& X, const int& Y );

	void ForceAroundClient( RECT client );

	int ErrorDialog( LPCTSTR message );
};

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

#endif //_WINDOW_H_