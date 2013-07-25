#include "System/System.h"
#include "StandardResources/resource.h"

System::System( void ) : log( "System" ), sceneManager( *this ), window( *this ), graphics( *this )
{
}

int System::Initialise( const HINSTANCE hInstance, const LPSTR lpCmdLine, const int nCmdShow )
{
	int result = window.Create( hInstance, lpCmdLine, nCmdShow );
	graphics.Initialise();
	//input;
	//sound;

	return result;
}

int System::Run( void )
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

		sceneManager.Run();	//Game Loop
    }

	return msg.wParam;	//If message is quit, this will be 0. If there was an error, this will be -1
	//~The Message Loop
}

//If the System is shutting down, that means end of program, time to do cleanup
void System::Shutdown( void )
{
	graphics.CleanUp();

	window.Destroy();
}

LRESULT CALLBACK System::MessageHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
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
							sceneManager.SetUpdate( true );
							sceneManager.SetRender( true );
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

System::~System( void )
{
	graphics.CleanUp();

	window.Destroy();
}