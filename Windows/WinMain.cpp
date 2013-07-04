#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN

#include "SceneManager\SceneManager.h"

SceneManager* Game = NULL;

//WinMain, Parameters: hInstance( application instance ), hPrevInstance( previous instance of the application running, unused holdover from win16 ), 
//lpCmdLine( command line arguments, not including program name ), nCmdShow( specifies initial window state, i.e. minimised, maximised, this is set in shortcut properties )
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	Game = SceneManager::Get();

	try
	{
		if( !Game )
		{
			return 0;
		}
	}
	catch( LPCTSTR error )
	{
		MessageBox( NULL, error, TEXT( "Error during construction" ), MB_ICONEXCLAMATION | MB_OK );
		if( Game != NULL )
		{
			Game->Quit();
		}
	}

	try
	{
		if( !Game->Initialise( hInstance, lpCmdLine, nCmdShow ) )
		{
			return 0;
		}
	}
	catch( LPCTSTR error )
	{
		MessageBox( NULL, error, TEXT( "Error during initialisation" ), MB_ICONEXCLAMATION | MB_OK );
		if( Game != NULL )
		{
			Game->Quit();
		}
	}

	int result = 0;
	try
	{
		//Run() implements the message loop, which is the main loop
		result = Game->Run();
	}
	catch( LPCTSTR error )
	{
		MessageBox( NULL, error, TEXT( "Error during main loop" ), MB_ICONEXCLAMATION | MB_OK );
		if( Game != NULL )
		{
			Game->Quit();
		}
	}

	try
	{
		Game->Shutdown();
	}
	catch( LPCTSTR error )
	{
		MessageBox( NULL, error, TEXT( "Error during shutdown" ), MB_ICONEXCLAMATION | MB_OK );
		if( Game != NULL )
		{
			Game->Quit();
		}
	}

	return result;
}

//Window Procedure: This processes messages for the window, Parameters: hWnd( handle to the window ), msg( message being processed ), wParam( and lParam are additional information
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_CLOSE: //Sent when X is clicked, or Alt-F4
			DestroyWindow( hWnd ); //Send WM_DESTROY
			break;
		case WM_DESTROY: //Window is being destroyed, child windows are destroyed first
			PostQuitMessage( 0 ); //Send WM_QUIT
			break;
		default: 
			return Game->MessageHandler( hWnd, msg, wParam, lParam );
	}
	return 0;
}