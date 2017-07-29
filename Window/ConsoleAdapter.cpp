#include "Window/ConsoleAdapter.h"
#include <windows.h>
#include <Fcntl.h>
#include <io.h>
#include <iostream>
#include <exception>

ConsoleAdapter::ConsoleAdapter( bool Output, bool Input ) : RedirectOutput( Output ), RedirectInput( Input )
{
}

ConsoleAdapter::~ConsoleAdapter( void )
{
	DestroyConsole();
}

void ConsoleAdapter::CreateConsole( bool Output, bool Input )
{
	RedirectOutput = Output;
	RedirectInput = Input;

	CreateConsole();
}

void ConsoleAdapter::CreateConsole( void )
{
	//Attempt to attach to parent console (if it exists)
	if( !AttachConsole( ATTACH_PARENT_PROCESS ) )
	{
		//If there is no parent console, create one
		if( !AllocConsole() )
		{
			throw std::exception( "AllocConsole has failed" );
		}
	}
	
	SetupStandardStreams();
}

void ConsoleAdapter::DestroyConsole( void )
{
	if( RedirectInput )
	{
		std::fclose( stdin );
	}

	if( RedirectOutput )
	{
		std::fclose( stdout );
	}

	if( !FreeConsole() )
	{
		if( GetLastError() != ERROR_INVALID_PARAMETER )
		{
			throw std::exception( "FreeConsole has failed" );
		}
	}
}

void ConsoleAdapter::SetupStandardStreams( void )
{
	if( RedirectInput )
	{
		//Setup stdin
		std::freopen("CONIN$", "r", stdin);

		std::cin.clear();
	}

	if( RedirectOutput )
	{
		//Setup stdout
		std::freopen("CONOUT$", "w", stdout);

		std::cout.clear();
	}
}