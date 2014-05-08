#define settingsFile "C:/Users/Scott/Programming/Projects/Scott_Oliver_Engine/Scott_Oliver_Engine/Settings/MainSettings.json"

#include "System/System.h"
#include "StandardResources/resource.h"

System::System( void ) : log( "System" ), settings( settingsFile, "/" ), sceneManager( *this ), window( *this ), graphics( *this ), input( *this ), assets( *this )
{
}

System::~System( void )
{
}

int System::Initialise( const HINSTANCE hInstance, const LPSTR lpCmdLine, const int nCmdShow )
{
	int result = window.Create( hInstance, lpCmdLine, nCmdShow );
	
	input.Init();
	graphics.Init();
	sound.Init();
	sceneManager.Init();

	return result;
}

int System::Run( void )
{
	//The Message Loop
	MSG msg;				//A system message

	while( TRUE )
    {
    	time.frameBegin();

        while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )	//Get message from queue
        {
            TranslateMessage( &msg );	//Additional Processing
            DispatchMessage( &msg );	//Send the Message
        }

		if( ::GetAsyncKeyState( VK_ESCAPE ) )
			PostMessage( window.getHandle(), WM_CLOSE, 0, 0 );

		GameLoop();

		time.frameEnd();

		if( msg.message == WM_QUIT )
		{
			sceneManager.Shutdown();
			break;
		}
    }

	return msg.wParam;	//If message is quit, this will be 0. If there was an error, this will be -1
	//~The Message Loop
}

int System::GameLoop( void )
{
	time.AddToAcc( time.deltaTimeS() );

	input.Update();
	sound.Update();
	sceneManager.Update();

	while( time.Acc() > time.fixedStepS() )
	{
		if( time.fixedStepS() == 0 ){ break; }
		//physics.Integrate( time.fixedStepS() );
		sceneManager.FixedUpdate();
		time.SubFromAcc( time.fixedStepS() );
	}

	input.PostUpdate();

	sceneManager.Render();

	return 0;
}

//If the System is shutting down, that means end of program, time to do cleanup
void System::Shutdown( void )
{
	//sceneManager.Shutdown();
	assets.CleanUp();
	sound.CleanUp();
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
				window.SetClientPosition( xPos, yPos );
				break;
			}
		case WM_MOVING:	//Remember new position //WINDOW AREA
			{
				RECT* temp = (RECT*)lParam;
				window.SetWindowPosition( ( temp->left ), ( temp->top ) );
				return true;
			}
		case WM_COMMAND:	//Sent by command items, e.g. menu options
			switch( LOWORD( wParam ) )
			{
				/*
				case ID_FILE_EXIT:
					Quit();
					break;
				//*/
					default:
					{
						return DefWindowProc( hWnd, msg, wParam, lParam );
					}
			}
			return DefWindowProc( hWnd, msg, wParam, lParam );
			break;
		case WM_KEYDOWN:
			{
				switch( wParam )
				{
					case VK_UP:
						log.Message( "Up received" );
						graphics.ToggleFullscreen();
						break;
				}
				break;
			}
		case WM_INPUT:
		{
			RAWINPUT raw;
			UINT dwSize = sizeof( raw );

			GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, &raw, &dwSize, sizeof( RAWINPUTHEADER ) );

			input.ReceiveRawInput( &raw );
			break;
		}
		default:
			return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return 0;
}