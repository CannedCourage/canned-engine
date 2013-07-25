#include "Window/WindowMS.h"
#include "StandardResources/resource.h"
#include "System/System.h"

#define FULLSCREEN		WS_EX_TOPMOST | WS_POPUP
#define WINDOWED2		WS_OVERLAPPEDWINDOW
#define WINDOWED		( WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE )

Window::Window( System &s ) : iWindow( s )
{
}

Window::~Window( void )
{
	Destroy();
}

HWND const Window::getHandle( void ) const
{
	return hwnd;
}

int Window::Create( const HINSTANCE hInstance, const LPSTR lpCmdLine, const int nCmdShow )
{
	//Register the window class
	windowClass.cbSize = sizeof( WNDCLASSEX );									//The size of the structure
	windowClass.style = CS_HREDRAW | CS_VREDRAW;								//The Class Style
	windowClass.lpfnWndProc = WndProc;											//Pointer to Function that points to the Windows Procedure
	windowClass.cbClsExtra = 0;													//Extra data allocated for this class in memory, usually 0
	windowClass.cbWndExtra = 0;													//Extra data allocated in memory PER WINDOW of this type, usually 0
	windowClass.hInstance = hInstance;											//Handle to application instance
	windowClass.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_MYICON ) );	//Large Icon (Alt-Tab) LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON))
	windowClass.hCursor = LoadCursor( hInstance, IDC_ARROW );					//Cursor displayed when over the window
	windowClass.hbrBackground = NULL;
	//windowClass.hbrBackground = ( HBRUSH )( COLOR_WINDOW+1 );					//Background brush that sets the colour of the background
	windowClass.lpszMenuName = MAKEINTRESOURCE( IDR_MENU1 );					//Name of a menu resource the windows of this class use
	windowClass.lpszClassName = "GameWindow";								//Class identifier
	windowClass.hIconSm = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_MYICON ) );	//Small Icon (Taskbar, etc)

	if( !RegisterClassEx( &windowClass ) )
	{
		MessageBox( NULL, TEXT( "Window Registration Failed :(" ), TEXT( "Error!" ), MB_ICONEXCLAMATION | MB_OK );
		return 0;
	}
	//~Register the window class

	//Create The Window
	DWORD style;
	RECT adjusted = { settings.client.x, settings.client.y, ( settings.client.x + settings.client.width ), ( settings.client.y + settings.client.height ) };

	if( settings.client.fullscreen )
	{
		style = FULLSCREEN;

		Cursor(false);
	}
	else
	{
		style = WINDOWED;

		Cursor(true);

		AdjustWindowRectEx( &adjusted, style, true, 0 );

		settings.window.x = adjusted.left;
		settings.window.y = adjusted.top;
		settings.window.width = ( adjusted.right - adjusted.left );
		settings.window.height = ( adjusted.bottom - adjusted.top );
	}

	//					   Extended Window Style, class to use, title bar text, Window Style, x, y, width, height, parent window, menu handle, app instance, creation data
	hwnd = CreateWindowEx( 0, "GameWindow", "ScottEngine", style, settings.window.x, settings.window.y, settings.window.width, settings.window.height, NULL, NULL, hInstance, NULL );

	if( hwnd == NULL )
	{
		MessageBox( hwnd, "Window Creation Failed! :(", "Error!", MB_ICONEXCLAMATION | MB_OK );
		return 0;
	}

	ShowWindow( hwnd, nCmdShow );	//Show window, initial state determined by nCmdShow
	UpdateWindow( hwnd );			//Redraw

	//~Create The Window
	return 1;

	Window::hInstance = hInstance;
}

void Window::Destroy( void )
{
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

	if( hwnd != NULL )
	{
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
}

int Window::Recreate( void )
{
	return 0;
}

void Window::Update( void )
{
	MoveWindow( hwnd, windowX, windowY, windowWidth, windowHeight, true );
	SetWindowLong( hwnd, GWL_STYLE, WINDOWED );
	//Don't change position, size, and don't activate the window, but stop being topmost
	SetWindowPos( hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_DRAWFRAME );
}

void Window::SetWindowPosition( const int& X, const int& Y )
{
	windowX = X;
	windowY = Y;
}

void Window::SetWindowSize( const int& Width, const int& Height )
{
	windowWidth = Width;
	windowHeight = Height;
}

void Window::Cursor( const bool& show )
{
	if( show )
	{
		while ( (ShowCursor(true) < 0) );
	}
	else
	{
		while ( (ShowCursor(false) >= 0) );
	}
}

void Window::MoveCursor( const int& X, const int& Y )
{
	SetCursorPos( X, Y );
}

int Window::ErrorDialog( LPCTSTR message )
{
	return MessageBox( hwnd, message, TEXT( "Error" ), MB_ICONEXCLAMATION | MB_OK );
}