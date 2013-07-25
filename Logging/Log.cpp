#define _CRT_SECURE_NO_WARNINGS 1
#include "Logging/Log.h"
#include <iostream>
#include <time.h>

#include <Windows.h>

using std::cout;
using std::endl;

const char* Log::globalFilename = DEFAULT_LOG_FILENAME;
FILE* Log::globalFile = NULL;
int Log::counter = 0;

Log::Log( void )
{
	UpdateTime();

	if( counter == 0 )
	{
		char temp[MAX_PATH];
		sprintf( temp, "%s.txt", globalFilename );				//Append .txt
		globalFile = fopen( temp, "a" );						//Create master file
		if( globalFile == NULL )
		{
			throw( TEXT( "Log: globalFile creation failed" ) );
		}
		fprintf( globalFile, "%s\t%s", temp, asctime( time_GMT ) );	//Print master filename - tab - timestamp
	}
	counter++;
}

Log::Log( const char* scope_ )
{
	UpdateTime();

	if( counter == 0 )
	{
		char temp[MAX_PATH];
		sprintf( temp, "%s.txt", globalFilename );				//Append .txt
		globalFile = fopen( temp, "a" );						//Create master file
		if( globalFile == NULL )
		{
			throw( TEXT( "Log: globalFile creation failed" ) );
		}
		fprintf( globalFile, "%s\t%s", temp, asctime( time_GMT ) );	//Print master filename - tab - timestamp to master file
	}

	//sprintf( scope, "%s", scope_ );								//Store scope for prepending messages
	scope = scope_;
	char tempscope[MAX_PATH];
	sprintf( tempscope, "Logs/%s.txt", scope_ );				//Derive filename from scope, i.e. append .txt and copy
	localFile = fopen( tempscope, "w" );						//Open log file for "scope.txt"
	if( localFile == NULL )
	{
		throw( TEXT( "Log: localFile creation failed" ) );
	}
	fprintf( localFile, "%s\t%s", tempscope, asctime( time_GMT ) );	//Print local filename - tab - timestamp to local file
	counter++;
}

Log::~Log(void)
{
	Message("Log Closing");
	if( localFile != NULL )
	{
		fclose(localFile);
		localFile = NULL;
	}

	if( counter == 1 )
	{
		if( globalFile != NULL )
		{
			UpdateTime();
			fprintf( globalFile, "\n%s: %s\t%s", DEFAULT_LOG_FILENAME, "Final Log Closing", asctime( time_GMT ) );
			fclose(globalFile);
			globalFile = NULL;
		}
	}

	counter--;
}

void Log::Message( const char* s, const bool show, const bool print )
{
	UpdateTime();

	if( show )
	{
		Show(s);
	}

	if( print )
	{
		Print(s);
	}
}

void Log::Show(const char *s)
{
	char temp[MAX_PATH];
	sprintf( temp, "\n%s: %s", scope, s );
	OutputDebugStringA(temp);	//Show in debug output
}

void Log::Print(const char *s)
{
	if( strlen(s) != 0 )
	{
		fprintf( globalFile, "\n%s: %s\t%s", scope, s, asctime( time_GMT ) );	//Print to master file
		fprintf( localFile, "\n%s: %s\t%s", scope, s, asctime( time_GMT ) );	//Print to local file
	}
}

void Log::UpdateTime( void )
{
	rawTime = time(NULL);
	time_GMT = localtime( &rawTime );
}

//#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
//#define  _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 1
//#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 0
//ifdef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
//#undef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
//#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
//#endif //_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES