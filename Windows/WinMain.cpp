#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "System/System.h"

#include <stdexcept>

System* sys = NULL;

//WinMain, Parameters: hInstance( application instance ), hPrevInstance( previous instance of the application running, unused holdover from win16 ), 
//lpCmdLine( command line arguments, not including program name ), nCmdShow( specifies initial window state, i.e. minimised, maximised, this is set in shortcut properties )
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	sys = new System();

	try
	{
		if( !sys )
		{
			MessageBox( NULL, TEXT( "Error during construction" ), TEXT( "Error during construction" ), MB_ICONEXCLAMATION | MB_OK );
			return 0;
		}
	}
	catch( std::exception error )
	{
		MessageBox( NULL, error.what(), TEXT( "Error during construction" ), MB_ICONEXCLAMATION | MB_OK );
		if( sys != NULL )
		{
			sys->Quit();
		}
	}
	catch( std::string error )
	{
		MessageBox( NULL, error.c_str(), TEXT( "Error during construction" ), MB_ICONEXCLAMATION | MB_OK );
		if( sys != NULL )
		{
			sys->Quit();
		}
	}

	try
	{
		if( !sys->Initialise( hInstance, lpCmdLine, nCmdShow ) )
		{
			MessageBox( NULL, TEXT( "Error during initialisation" ), TEXT( "Error during initialisation" ), MB_ICONEXCLAMATION | MB_OK );
			return 0;
		}
	}
	catch( std::exception error )
	{
		MessageBox( NULL, error.what(), TEXT( "Error during initialisation" ), MB_ICONEXCLAMATION | MB_OK );
		if( sys != NULL )
		{
			sys->Quit();
		}
	}
	catch( std::string error )
	{
		MessageBox( NULL, error.c_str(), TEXT( "Error during initialisation" ), MB_ICONEXCLAMATION | MB_OK );
		if( sys != NULL )
		{
			sys->Quit();
		}
	}

	int result = 0;
	try
	{
		//Run() implements the message loop, which is the main loop
		result = sys->Run();
	}
	catch( std::exception error )
	{
		MessageBox( NULL, error.what(), TEXT( "Error during main loop" ), MB_ICONEXCLAMATION | MB_OK );
		if( sys != NULL )
		{
			sys->Quit();
		}
	}
	catch( std::string error )
	{
		MessageBox( NULL, error.c_str(), TEXT( "Error during main loop" ), MB_ICONEXCLAMATION | MB_OK );
		if( sys != NULL )
		{
			sys->Quit();
		}
	}

	try
	{
		sys->Shutdown();
	}
	catch( std::exception error )
	{
		MessageBox( NULL, error.what(), TEXT( "Error during shutdown" ), MB_ICONEXCLAMATION | MB_OK );
		if( sys != NULL )
		{
			sys->Quit();
		}
	}
	catch( std::string error )
	{
		MessageBox( NULL, error.c_str(), TEXT( "Error during shutdown" ), MB_ICONEXCLAMATION | MB_OK );
		if( sys != NULL )
		{
			sys->Quit();
		}
	}

	if( sys != NULL )
	{
		delete sys;
		sys = NULL;
	}

	return result;
}

//Window Procedure: This processes messages for the window, Parameters: hWnd( handle to the window ), msg( message being processed ), wParam and lParam are additional information
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
			return sys->MessageHandler( hWnd, msg, wParam, lParam );
	}
	return 0;
}