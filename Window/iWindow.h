#ifndef _IWINDOW_H_
#define _IWINDOW_H_

#include "Logging/Log.h"

class System;

class iWindow
{
private:
protected:

	Log log;
	System& system;

	int windowX = 0, windowY = 0, windowWidth = 0, windowHeight = 0;
	int clientX = 0, clientY = 0, clientWidth = 0, clientHeight = 0;
	bool fullscreen = false;
public:

	iWindow( System &s );
	~iWindow( void ) = default;

	//virtual int Create( void ) = 0;
	//virtual void Destroy( void ) = 0;
	
	virtual int Recreate( void ) = 0;

	virtual void Update( void ) = 0;

	virtual void SetWindowPosition( const int& X, const int& Y ) = 0;
	virtual void SetWindowSize( const int& Width, const int& Height ) = 0;
	virtual void SetClientPosition( const int& X, const int& Y ) = 0;
	virtual void SetClientSize( const int& Width, const int& Height ) = 0;

	virtual void CursorVisible( const bool& show ) = 0;
	virtual void MoveCursor( const int& X, const int& Y ) = 0;

	virtual int Dialog( char* message, char* title ) = 0;
};

#endif //_IWINDOW_H_