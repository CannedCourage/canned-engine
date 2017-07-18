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

void ConsoleAdapter::CreateConsole( void )
{
	if( !AllocConsole() )
	{
		throw std::exception( "AllocConsole has failed" );
	}
	else
	{
		SetupStandardStreams();
	}
}

void ConsoleAdapter::SetupStandardStreams( void )
{
	if( RedirectInput )
	{
		//Setup stdin
		HANDLE handle_stdin = GetStdHandle( STD_INPUT_HANDLE );

		int fileDesc_stdin = _open_osfhandle( (long)handle_stdin, _O_TEXT );

		if( fileDesc_stdin == -1 )
		{
			throw std::exception( "fileDesc_stdin is not valid" );
		}

		FILE* new_stdin = _fdopen( fileDesc_stdin, "w" );

		if( !new_stdin )
		{
			throw std::exception( "new_stdin is not valid" );
		}

		// printf( typeid( *stdin ).name() );
		FILE old_stdin = *stdin;
		*stdin = *new_stdin;

		std::cin.clear();
	}

	if( RedirectOutput )
	{
		//Setup stdout
		HANDLE handle_stdout = GetStdHandle( STD_OUTPUT_HANDLE );

		int fileDesc_stdout = _open_osfhandle( (long)handle_stdout, _O_TEXT );

		if( fileDesc_stdout == -1 )
		{
			throw std::exception( "fileDesc_stdout is not valid" );
		}

		FILE* new_stdout = _fdopen( fileDesc_stdout, "w" );

		if( !new_stdout )
		{
			throw std::exception( "new_stdout is not valid" );
		}

		// printf( typeid( *stdout ).name() );
		FILE old_stdout = *stdout;
		*stdout = *new_stdout;

		std::cout.clear();
	}
}

void ConsoleAdapter::DestroyConsole( void )
{
	if( !FreeConsole() )
	{
		if( GetLastError() != ERROR_INVALID_PARAMETER )
		{
			throw std::exception( "FreeConsole has failed" );
		}
	}
}

void ConsoleAdapter::AttachParentConsole( void )
{
	if( !AttachConsole( ATTACH_PARENT_PROCESS ) )
	{
		throw std::exception( "AttachConsole has failed" );
	}
	else
	{
		SetupStandardStreams();
	}
}