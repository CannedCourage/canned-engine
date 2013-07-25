#ifndef _WINDOWMS_H_
#define _WINDOWMS_H_

#include "Window/iWindow.h"
#include <Windows.h>

class WindowMS : iWindow
{
private:
protected:

	HWND hwnd;				//Handle
	WNDCLASSEX windowClass;	//A structure that stores information used to create the window
	HINSTANCE hInstance;
	HWND graphicsDialog, colourDialog, depthDialog, sampleDialog, displayDialog, aspectDialog;
	HWND goButton;
public:

	Window( System &s );
	~Window( void );

	HWND const getHandle( void ) const;

	int Create( const HINSTANCE hInstance, const LPSTR lpCmdLine, const int nCmdShow );
	void Destroy( void );
};

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

#endif //_WINDOWMS_H_