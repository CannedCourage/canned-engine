#include "Window/WindowMS.h"
#include "StandardResources/resource.h"
#include "System/System.h"

#define FULLSCREEN		WS_EX_TOPMOST | WS_POPUP
#define WINDOWED2		WS_OVERLAPPEDWINDOW
#define WINDOWED		( WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE )
//WS_SYSMENU

WindowMS::WindowMS( System &s ) : iWindow( s )
{
}

WindowMS::~WindowMS( void )
{
	Destroy();
}

HWND const WindowMS::getHandle( void ) const
{
	return hwnd;
}

int WindowMS::Create( const HINSTANCE hInstance, const LPSTR lpCmdLine, const int nCmdShow )
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
	windowClass.lpszMenuName = NULL;
	//windowClass.lpszMenuName = MAKEINTRESOURCE( IDR_MENU1 );					//Name of a menu resource the windows of this class use
	windowClass.lpszClassName = "GameWindow";									//Class identifier
	windowClass.hIconSm = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_MYICON ) );	//Small Icon (Taskbar, etc)

	if( !RegisterClassEx( &windowClass ) )
	{
		MessageBox( NULL, TEXT( "Window Registration Failed :(" ), TEXT( "Error!" ), MB_ICONEXCLAMATION | MB_OK );

		return 0;
	}
	//~Register the window class

	//Create The Window
	DWORD style;

	if( fullscreen )
	{
		style = FULLSCREEN;

		CursorVisible( false );

		SetWindowPosition( 0, 0 );

		SetWindowSize( clientWidth, clientHeight );
	}
	else
	{
		style = WINDOWED;

		CursorVisible( true );

		AdjustWindow();
	}

	//					   Extended Window Style, class to use, title bar text, Window Style, x, y, width, height, parent window, menu handle, app instance, creation data
	hwnd = CreateWindowEx( 0, "GameWindow", "ScottEngine", style, windowX, windowY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL );

	if( hwnd == NULL )
	{
		MessageBox( hwnd, "Window Creation Failed! :(", "Error!", MB_ICONEXCLAMATION | MB_OK );
		return 0;
	}

	ShowWindow( hwnd, nCmdShow );	//Show window, initial state determined by nCmdShow
	UpdateWindow( hwnd );			//Redraw

	//~Create The Window
	return 1;

	WindowMS::hInstance = hInstance; //Huh? What is this...I don't even...
}

void WindowMS::Destroy( void )
{
	UnregisterClass( windowClass.lpszClassName, windowClass.hInstance );

	if( hwnd != NULL )
	{
		DestroyWindow( hwnd );
		hwnd = NULL;
	}
}

int WindowMS::Recreate( void )
{
	return 0;
}

void WindowMS::Update( void )
{
	AdjustWindow();

	MoveWindow( hwnd, windowX, windowY, windowWidth, windowHeight, true );

	SetWindowLong( hwnd, GWL_STYLE, WINDOWED );

	//Don't change position, size, and don't activate the window, but stop being topmost
	SetWindowPos( hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_DRAWFRAME );
}

void WindowMS::SetWindowPosition( const int& X, const int& Y )
{
	windowX = X;
	windowY = Y;
}

void WindowMS::SetWindowSize( const int& Width, const int& Height )
{
	windowWidth = Width;
	windowHeight = Height;
}

void WindowMS::SetClientPosition( const int& X, const int& Y )
{
	clientX = X;
	clientY = Y;

	settings.SetInteger( "display/x", clientX );
	settings.SetInteger( "display/y", clientY );
}

void WindowMS::SetClientSize( const int& Width, const int& Height )
{
	clientWidth = Width;
	clientHeight = Height;
}

void WindowMS::CursorVisible( const bool& show )
{
	if( show )
	{
		while ( ( ShowCursor( true ) < 0 ) );
	}
	else
	{
		while ( ( ShowCursor( false ) >= 0 ) );
	}
}

void WindowMS::MoveCursor( const int& X, const int& Y )
{
	SetCursorPos( X, Y );
}

int WindowMS::Dialog( char* message, char* title )
{
	return MessageBox( hwnd, message, title, MB_ICONEXCLAMATION | MB_OK );
}

void WindowMS::AdjustWindow( void )
{
	DWORD style = WINDOWED;

	clientX = settings.GetInteger( "display/x" );
	clientY = settings.GetInteger( "display/y" );
	clientWidth = settings.GetInteger( "display/xResolution" );
	clientHeight = settings.GetInteger( "display/yResolution" );

	RECT adjusted = { clientX, clientY, ( clientX + clientWidth ), ( clientY + clientHeight ) };

	AdjustWindowRectEx( &adjusted, style, false, 0 );

	//Sanity Check Window Position
	if( adjusted.left < 0 )
	{
		adjusted.left = 0;
	}

	if( adjusted.top < 0 )
	{
		adjusted.top = 0;
	}

	SetWindowPosition( adjusted.left, adjusted.top );

	SetWindowSize( ( adjusted.right - adjusted.left ), ( adjusted.bottom - adjusted.top ) );
}