//Window class
//GLFW window creation and management

#ifndef _WINDOWGLFW_H_
#define _WINDOWGLFW_H_

class GLFWwindow;
class System;

class WindowGLFW
{
private:
protected:

	GLFWwindow* Window;
public:

	WindowGLFW( void );
	~WindowGLFW( void );

	void Update( void );

	bool IsValid( void );

	GLFWwindow* GetWindow( void );

	void SetFullscreen( const bool Fullscreen );
	void SetWindowPosition( int X, int Y );
	void SetWindowSize( int Width, int Height );
	void SetCursorVisible( const bool Show );
	void MoveCursor( int X, int Y );
	void Quit( void );
};

#endif //_WINDOWGLFW_H_