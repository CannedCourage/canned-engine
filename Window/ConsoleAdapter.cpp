#include "Window/ConsoleAdapter.h"
#include <windows.h>
#include <Fcntl.h>
#include <io.h>
#include <iostream>

ConsoleAdapter::ConsoleAdapter( unsigned long type ) : ConsoleType( type )
{
}

ConsoleAdapter::~ConsoleAdapter( void )
{
}

void ConsoleAdapter::CreateConsole( void )
{
	if( !AllocConsole() )
	{
		throw "AllocConsole has failed";
	}
	else
	{
		SetupStandardStreams();
	}
}

void ConsoleAdapter::SetupStandardStreams( void )
{
	if( ConsoleType & CONSOLE_INPUT )
	{
		//Setup stdin
		HANDLE handle_stdin = GetStdHandle( STD_INPUT_HANDLE );

		int fileDesc_stdin = _open_osfhandle( (long)handle_stdin, _O_TEXT );

		if( fileDesc_stdin == -1 )
		{
			throw "fileDesc_stdin is not valid";
		}

		FILE* new_stdin = _fdopen( fileDesc_stdin, "w" );

		if( !new_stdin )
		{
			throw "new_stdin is not valid";
		}

		// printf( typeid( *stdin ).name() );
		FILE old_stdin = *stdin;
		*stdin = *new_stdin;

		std::cin.clear();
	}

	if( ConsoleType & CONSOLE_OUTPUT )
	{
		//Setup stdout
		HANDLE handle_stdout = GetStdHandle( STD_OUTPUT_HANDLE );

		int fileDesc_stdout = _open_osfhandle( (long)handle_stdout, _O_TEXT );

		if( fileDesc_stdout == -1 )
		{
			throw "fileDesc_stdout is not valid";
		}
		
		FILE* new_stdout = _fdopen( fileDesc_stdout, "w" );

		if( !new_stdout )
		{
			throw "new_stdout is not valid";
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
		throw "FreeConsole has failed";
	}
}

void ConsoleAdapter::AttachParentConsole( void )
{
	if( !AttachConsole( ATTACH_PARENT_PROCESS ) )
	{
		throw "AttachConsole has failed";
	}
	else
	{
		SetupStandardStreams();
	}
}