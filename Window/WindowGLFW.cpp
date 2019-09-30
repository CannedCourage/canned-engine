#include <stdexcept>
#include <GLFW/glfw3.h>

#include "Window/WindowGLFW.h"

#pragma comment(lib, "glfw3dll.lib") //TODO: Move this to build script

WindowGLFW::WindowGLFW( void )
{
	glfwInit();

	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API ); //No OpenGL
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE ); //Prevent resizing

    Window = glfwCreateWindow( 800, 600, "GLFW window", nullptr, nullptr );

    if( !Window )
    {
        glfwTerminate();
        throw std::runtime_error( "Error creating GLFW window!" );
    }

    glfwMakeContextCurrent( Window );
}

WindowGLFW::~WindowGLFW( void )
{
	glfwDestroyWindow( Window );

	glfwTerminate();
}

void WindowGLFW::Update( void )
{
	/* Poll for and process events */
	glfwPollEvents();

	if( glfwGetKey( Window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
	{
		Quit();
	}
}

void WindowGLFW::SetFullscreen( const bool Fullscreen )
{
	//glfwSetWindowMonitor
}

void WindowGLFW::SetWindowPosition( int X, int Y )
{
	//glfwSetWindowPos
}

void WindowGLFW::SetWindowSize( int Width, int Height )
{
	//glfwSetWindowSize
}

void WindowGLFW::SetCursorVisible( const bool Show )
{
	if( Show )
	{
		glfwSetInputMode( Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
	}
	else
	{
		glfwSetInputMode( Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
	}
}

void WindowGLFW::Quit( void )
{
	glfwSetWindowShouldClose( Window, GL_TRUE );
}

bool WindowGLFW::IsValid( void )
{
	return !( glfwWindowShouldClose( Window ) );
}

GLFWwindow* WindowGLFW::GetWindow( void )
{
	return Window;
}