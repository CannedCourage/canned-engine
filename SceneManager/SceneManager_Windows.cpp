#include "SceneManager\SceneManager.h"
#include "Graphics\Screen.h"
#include "StandardResources\resource.h"

int SceneManager::Run( void )
{
	//The Message Loop
	MSG msg;				//A system message

	while( TRUE )
    {
        while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )	//Get message from queue
        {
            TranslateMessage( &msg );	//Additional Processing
            DispatchMessage( &msg );	//Send the Message
        }

		if( ::GetAsyncKeyState( VK_ESCAPE ) )
			PostMessage( window.getHandle(), WM_CLOSE, 0, 0 );

        if( msg.message == WM_QUIT )
            break;

		if( update )
		{
			if( !Update() )
			{
				break;
			}
			else
			{
				if( render )
				{
					Render();
				}
			}
		}
    }

	return msg.wParam;	//If message is quit, this will be 0. If there was an error, this will be -1
	//~The Message Loop
}

LRESULT CALLBACK SceneManager::MessageHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_CREATE:
			break;
		case WM_MOVE:	//Remember new position //CLIENT AREA
			{
				int xPos = ( int )LOWORD( lParam );
				int yPos = ( int )HIWORD( lParam );
				graphics.SetClientPosition( xPos, yPos );
				break;
			}
		case WM_MOVING:	//Remember new position //WINDOW AREA
			{
				RECT* temp = (RECT*)lParam;
				window.SetPosition( temp->left, temp->top );
				return true;
			}
		case WM_SIZE:	//Remember new size //THIS IS THE CLIENT AREA
			{
				int width = ( int )LOWORD( lParam );
				int height = ( int )HIWORD( lParam );
				graphics.SetClientSize( width, height );
				break;
			}
		case WM_SIZING:	//Remember new size //WINDOW AREA
			{
				RECT* temp = (RECT*)lParam;
				int width = temp->right - temp->left;
				int height = temp->bottom - temp->top;
				window.SetSize( width, height );
			}
		case WM_COMMAND:	//Sent by command items, e.g. menu options
			switch( LOWORD( wParam ) )
			{
				case ID_FILE_EXIT:
					Quit();
					break;
				case ID_VIEW_FULLSCREEN:
					graphics.SetFullscreen();
					break;
				case ID_ADAPTERSCOMBO:
					switch( HIWORD( wParam ) )
					{
						case CBN_SELCHANGE:
							int index = SendMessage( (HWND)lParam, (UINT)CB_GETCURSEL, 0, 0 );
							graphics.SelectAdapter( index );
							break;
					}
					break;
				case ID_COLOURCOMBO:
					switch( HIWORD( wParam ) )
					{
						case CBN_SELCHANGE:
							int index = SendMessage( (HWND)lParam, (UINT)CB_GETCURSEL, 0, 0 );
							graphics.SelectBufferFormat( index );
							break;
					}
					break;
				case ID_DEPTHCOMBO:
					switch( HIWORD( wParam ) )
					{
						case CBN_SELCHANGE:
							int index = SendMessage( (HWND)lParam, (UINT)CB_GETCURSEL, 0, 0 );
							graphics.SelectDepthFormat( index );
							break;
					}
					break;
				case ID_SAMPLECOMBO:
					switch( HIWORD( wParam ) )
					{
						case CBN_SELCHANGE:
							int index = SendMessage( (HWND)lParam, (UINT)CB_GETCURSEL, 0, 0 );
							graphics.SelectMultisample( index );
							break;
					}
					break;
				case ID_DISPLAYCOMBO:
					switch( HIWORD( wParam ) )
					{
						case CBN_SELCHANGE:
							int index = SendMessage( (HWND)lParam, (UINT)CB_GETCURSEL, 0, 0 );
							graphics.SelectResolution( index );
							break;
					}
					break;
				case ID_ASPECTCOMBO:
					switch( HIWORD( wParam ) )
					{
						case CBN_SELCHANGE:
							int index = SendMessage( (HWND)lParam, (UINT)CB_GETCURSEL, 0, 0 );
							graphics.SelectAspect( index );
							break;
					}
					break;
				case ID_GOBUTTON:
					switch( HIWORD( wParam ) )
					{
						case BN_CLICKED:
							window.DestroyDialogs();
							graphics.Reset();
							SetUpdate( true );
							SetRender( true );
							break;
					}
					break;
			}
			return DefWindowProc( hWnd, msg, wParam, lParam );
			break;
		case WM_KEYDOWN:
			{
				switch( wParam )
				{
					case VK_ESCAPE:
						Quit();
						break;
					case VK_UP:
						log.Message( "Up received" );
						graphics.ChangeView();
						break;
				}
				break;
			}
		default: 
			return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return 0;
}

/* This has moved to the Window class
int SceneManager::InitWindow( HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow )
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
	//windowClass.hbrBackground = ( HBRUSH )( COLOR_WINDOW+1 );					//Background brush that sets the colour of the background
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = MAKEINTRESOURCE( IDR_MENU1 );					//Name of a menu resource the windows of this class use
	windowClass.lpszClassName = windowClassName;								//Class identifier
	windowClass.hIconSm = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_MYICON ) );	//Small Icon (Taskbar, etc)

	if( !RegisterClassEx( &windowClass ) )
	{
		MessageBox( NULL, L"Window Registration Failed :(", L"Error!", MB_ICONEXCLAMATION | MB_OK );
		log.Message( "Window Registration Failed :(" );
		return 0;
	}
	//~Register the window class

	//Create The Window
	DWORD style;
	RECT adjusted = { settings.client.x, settings.client.y, ( settings.client.x + settings.client.width ), ( settings.client.y + settings.client.height ) };

	if( settings.client.fullscreen )
	{
		style = FULLSCREEN;

		while (ShowCursor(false) >= 0);
	}
	else
	{
		style = WINDOWED;

		AdjustWindowRectEx( &adjusted, style, true, 0 );

		settings.window.x = adjusted.left;
		settings.window.y = adjusted.top;
		settings.window.width = ( adjusted.right - adjusted.left );
		settings.window.height = ( adjusted.bottom - adjusted.top );
	}

	//					   Extended Window Style, class to use, title bar text, Window Style, x, y, width, height, parent window, menu handle, app instance, creation data
	hWnd = CreateWindowEx( 0, windowClassName, L"ScottEngine", style, settings.window.x, settings.window.y, settings.window.width, settings.window.height, NULL, NULL, hInstance, NULL );

	if( hWnd == NULL )
	{
		MessageBox( hWnd, L"Window Creation Failed! :(", L"Error!", MB_ICONEXCLAMATION | MB_OK );
		return 0;
	}

	ShowWindow( hWnd, nCmdShow );	//Show window, initial state determined by nCmdShow
	UpdateWindow( hWnd );			//Redraw

	//~Create The Window
	return 1;
}
//*/