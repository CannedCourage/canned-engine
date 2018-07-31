#include "Window/WindowGLFW.h"
#include "System/System.h"
#include "Graphics/GraphicsVK.h"

#include <stdexcept>

#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3dll.lib")

void key_callback( GLFWwindow* Window, int Key, int Scancode, int Action, int Mods )
{
	System* sys = static_cast<System*>( glfwGetWindowUserPointer( Window ) );

	sys->input.ReceiveKeyboardInput( Key, Scancode, Action, Mods );
}

void cursor_position_callback( GLFWwindow* Window, double X, double Y )
{
	System* sys = static_cast<System*>( glfwGetWindowUserPointer( Window ) );

	//sys->input.
}

void window_size_callback( GLFWwindow* Window, int Width, int Height )
{
	//Record in settings?
}

void framebuffer_size_callback( GLFWwindow* Window, int Width, int Height )
{
	//Update graphics service + record in settings
    //glViewport(0, 0, width, height);
}

WindowGLFW::WindowGLFW( void )
{
	glfwInit();
}

WindowGLFW::~WindowGLFW( void )
{
	glfwTerminate();
}

void WindowGLFW::Init( System* Sys )
{
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API ); //No OpenGL
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE ); //Prevent resizing

    Window = glfwCreateWindow( 800, 600, "GLFW window", nullptr, nullptr );
    glfwSetWindowUserPointer( Window, Sys ); //Make System available to GLFW

    if( !Window )
    {
        glfwTerminate();
        throw std::runtime_error( "Error creating GLFW window!" );
    }

    glfwMakeContextCurrent( Window );

    Sys->GetGraphics()->Window = Window;

    glfwSetKeyCallback( Window, key_callback );
    //Cursor Position callback
    //glfwSetWindowSizeCallback(Window, window_size_callback);
    //glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
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

void WindowGLFW::CleanUp( void )
{
	glfwDestroyWindow( Window );

    glfwTerminate();
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