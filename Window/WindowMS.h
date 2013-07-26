#ifndef _WINDOWMS_H_
#define _WINDOWMS_H_

#include "Window/iWindow.h"
#include <Windows.h>

class WindowMS : public iWindow
{
private:
protected:

	HWND hwnd;				//Handle
	WNDCLASSEX windowClass;	//A structure that stores information used to create the window
	HINSTANCE hInstance;
public:

	WindowMS( System &s );
	~WindowMS( void );

	HWND const getHandle( void ) const;

	int Create( const HINSTANCE hInstance, const LPSTR lpCmdLine, const int nCmdShow );
	void Destroy( void );

	virtual int Recreate( void );

	virtual void Update( void );

	virtual void SetWindowPosition( const int& X, const int& Y );
	virtual void SetWindowSize( const int& Width, const int& Height );
	virtual void SetClientPosition( const int& X, const int& Y );
	virtual void SetClientSize( const int& Width, const int& Height );

	virtual void CursorVisible( const bool& show );
	virtual void MoveCursor( const int& X, const int& Y );

	virtual int Dialog( char* message, char* title );
};

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

#endif //_WINDOWMS_H_