#define settingsFile "w:/engine/data/settings/MainSettings.json"

#include "System/System.h"
#include "StandardResources/resource.h"

System::System( void )
{
	std::ifstream mainSettingsFile{ settingsFile };
	
	mainSettingsFile >> GlobalSettings;
}

System::~System( void )
{
	std::ofstream mainSettingsFile{ settingsFile };

	mainSettingsFile << std::setw(4) << GlobalSettings << std::endl;
}

int System::Initialise( const HINSTANCE hInstance, const LPSTR lpCmdLine, const int nCmdShow )
{
	Console.CreateConsole( true, false );

	int result = window.Create( hInstance, lpCmdLine, nCmdShow );
	
	input.Init();
	graphics.Init();
	sound.Init();
	sceneManager.Init();

	newGraphics.Init();

	return result;
}

int System::Run( void )
{
	//The Message Loop
	MSG msg; //A system message

	while( TRUE )
    {
    	time.FrameBegin();

        while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )	//Get message from queue
        {
            TranslateMessage( &msg ); //Additional Processing
            DispatchMessage( &msg ); //Send the Message
        }

		if( ::GetAsyncKeyState( VK_ESCAPE ) )
		{
			Quit();
			time.FrameEnd();
			break;
		}

		if( msg.message == WM_QUIT )
		{
			TRACE( "QUITTING" );
			time.FrameEnd();
			break;
		}

		GameLoop();

		time.FrameEnd();
    }

	return msg.wParam;	//If message is quit, this will be 0. If there was an error, this will be -1
	//~The Message Loop
}

int System::GameLoop( void )
{
	time.AddToPhysicsAccumulator( time.DeltaTimeActual() );

	sceneManager.Prepare();

	input.Update();
	sceneManager.Update();

	while( time.GetPhysicsAccumulator() > time.FixedStepActual() )
	{
		if( time.FixedStepActual() == EngineDuration::zero() ){ break; }
		//physics.Integrate( time.FixedStep() );
		sceneManager.FixedUpdate();
		time.SubtractFromPhysicsAccumulator( time.FixedStepActual() );
	}

	input.PostUpdate();
	sound.Update();

	//sceneManager.Render();
	newGraphics.DrawFrame();

	return 0;
}

//If the System is shutting down, that means end of program, time to do cleanup
void System::Shutdown( void )
{
	newGraphics.CleanUp();
	sceneManager.Shutdown();
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
						TRACE( "Up received" );
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