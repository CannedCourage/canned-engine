#ifndef _IWINDOW_H_
#define _IWINDOW_H_

#include "Logging/Log.h"

class System;
class DefaultSettings;

class Window
{
private:
protected:

	Log log;

	DefaultSettings &settings;

	int windowX, windowY, windowWidth, windowHeight;
	int clientX, clientY, clientWidth, clientHeight;
	bool maximised;
public:

	Window( System &s )
	~Window( void )

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