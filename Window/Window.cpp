#include "Window\Window.h"
#include "Graphics\Screen.h"
#include "StandardResources\resource.h"
#include "System\System.h"

#define FULLSCREEN		WS_EX_TOPMOST | WS_POPUP
#define WINDOWED2		WS_OVERLAPPEDWINDOW
#define WINDOWED		( WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE )

Window::Window( System &s ) : ILoggable( "Window" ), settings( s.settings ),
	x( settings.window.x ), y( settings.window.y ), width( settings.window.width ), height( settings.window.height ), maximised( settings.window.maximised )
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
	windowClass.lpszClassName = L"GameWindow";								//Class identifier
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
	hwnd = CreateWindowEx( 0, L"GameWindow", L"ScottEngine", style, settings.window.x, settings.window.y, settings.window.width, settings.window.height, NULL, NULL, hInstance, NULL );

	if( hwnd == NULL )
	{
		MessageBox( hwnd, L"Window Creation Failed! :(", L"Error!", MB_ICONEXCLAMATION | MB_OK );
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

void Window::AddButton( void )
{
	goButton = CreateWindow( TEXT( "BUTTON" ), TEXT( "GO" ), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 300, 100, 100, 100, hwnd, (HMENU)ID_GOBUTTON, hInstance, NULL );
}

void Window::AdapterDialog( void )
{
	AddButton();

	graphicsDialog = CreateWindow( TEXT( "combobox" ), TEXT( "" ), WS_VSCROLL | WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 0, 0, 200, 100, hwnd, (HMENU)ID_ADAPTERSCOMBO, hInstance, NULL );
	colourDialog = CreateWindow( TEXT( "combobox" ), TEXT( "" ), WS_VSCROLL | WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 0, 50, 200, 100, hwnd, (HMENU)ID_COLOURCOMBO, hInstance, NULL );
	depthDialog = CreateWindow( TEXT( "combobox" ), TEXT( "" ), WS_VSCROLL | WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 0, 100, 200, 200, hwnd, (HMENU)ID_DEPTHCOMBO, hInstance, NULL );
	sampleDialog = CreateWindow( TEXT( "combobox" ), TEXT( "" ), WS_VSCROLL | WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 0, 150, 200, 200, hwnd, (HMENU)ID_SAMPLECOMBO, hInstance, NULL );
	displayDialog = CreateWindow( TEXT( "combobox" ), TEXT( "" ), WS_VSCROLL | WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 0, 250, 300, 300, hwnd, (HMENU)ID_DISPLAYCOMBO, hInstance, NULL );
	aspectDialog = CreateWindow( TEXT( "combobox" ), TEXT( "" ), WS_VSCROLL | WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 0, 200, 300, 300, hwnd, (HMENU)ID_ASPECTCOMBO, hInstance, NULL );
	if( graphicsDialog == NULL || colourDialog == NULL || depthDialog == NULL )
	{
		throw( TEXT( "Dialog creation failed" ) );
	}
	SendMessage( graphicsDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "Select Adapter" ) );
	SendMessage( graphicsDialog, CB_SETCURSEL, 0, 0 );

	SendMessage( colourDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "Colour Format" ) );
	SendMessage( colourDialog, CB_SETCURSEL, 0, 0 );
	SendMessage( colourDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "32-bit" ) );
	SendMessage( colourDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "24-bit" ) );
	SendMessage( colourDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "16-bit" ) );

	SendMessage( depthDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "Depth Format" ) );
	SendMessage( depthDialog, CB_SETCURSEL, 0, 0 );
	SendMessage( depthDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "D32" ) );
	SendMessage( depthDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "D24S8" ) );
	SendMessage( depthDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "D15S1" ) );
	SendMessage( depthDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "D16" ) );

	SendMessage( sampleDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "Anti-aliasing" ) );
	SendMessage( sampleDialog, CB_SETCURSEL, 0, 0 );
	SendMessage( sampleDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "Off" ) );
	SendMessage( sampleDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "2 Samples" ) );
	SendMessage( sampleDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "4 Samples" ) );
	SendMessage( sampleDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "8 Samples" ) );
	SendMessage( sampleDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "16 Samples" ) );

	SendMessage( displayDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "Select Display Mode" ) );
	SendMessage( displayDialog, CB_SETCURSEL, 0, 0 );

	SendMessage( aspectDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "Select Aspect Ratio" ) );
	SendMessage( aspectDialog, CB_SETCURSEL, 0, 0 );
	SendMessage( aspectDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "4:3" ) );
	SendMessage( aspectDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "16:9" ) );
	SendMessage( aspectDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "16:10" ) );
}

void Window::AddAdapterOption( const char* adapter )
{
	if( graphicsDialog != NULL )
	{
		SendMessageA( graphicsDialog, CB_ADDSTRING, 0, (LPARAM)adapter );
	}
}

void Window::AddDisplayOption( const char* adapter )
{
	if( displayDialog != NULL )
	{
		SendMessageA( displayDialog, CB_ADDSTRING, 0, (LPARAM)adapter );
	}
}

void Window::ClearDisplayOptions( void )
{
	if( displayDialog != NULL )
	{
		SendMessageA( displayDialog, CB_RESETCONTENT, 0, 0 );
		SendMessage( displayDialog, CB_ADDSTRING, 0, (LPARAM)TEXT( "Select Display Mode" ) );
		SendMessage( displayDialog, CB_SETCURSEL, 0, 0 );
	}
}

void Window::DestroyDialogs( void )
{
	DestroyWindow( graphicsDialog );
	DestroyWindow( colourDialog );
	DestroyWindow( depthDialog );
	DestroyWindow( sampleDialog );
	DestroyWindow( displayDialog );
	DestroyWindow( aspectDialog );
	DestroyWindow( goButton );
}

void Window::Update( void )
{
	MoveWindow( hwnd, x, y, width, height, true );
	SetWindowLong( hwnd, GWL_STYLE, WINDOWED );
	//Don't change position, size, and don't activate the window, but stop being topmost
	SetWindowPos( hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_DRAWFRAME );

	/*UpdateWindow( hwnd );
	UpdateWindow( graphicsDialog );
	UpdateWindow( colourDialog );
	UpdateWindow( depthDialog );
	UpdateWindow( sampleDialog );
	UpdateWindow( displayDialog );
	UpdateWindow( aspectDialog );*/
}

void Window::SetPosition( const int& X, const int& Y )
{
	settings.window.x = x = X;
	settings.window.y = y = Y;
}

void Window::SetSize( const int& Width, const int& Height )
{
	settings.window.width = width = Width;
	settings.window.height = height = Height;
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

void Window::ForceAroundClient( RECT client )
{
	AdjustWindowRect( &client, WINDOWED, false );

	SetPosition( client.left, client.top );
	SetSize( ( client.right - client.left ), ( client.bottom - client.top ) );

	//Update Window?
}

int Window::ErrorDialog( LPCTSTR message )
{
	return MessageBox( hwnd, message, TEXT( "Error" ), MB_ICONEXCLAMATION | MB_OK );
}