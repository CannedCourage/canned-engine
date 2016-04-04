//#define _CRT_SECURE_NO_WARNINGS 1
#include "Logging/Log.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <exception>

/*
If a method/function expects a const reference to string, it will also accept a string literal, e.g.
 
void Method( const std::string& str );
a.Method( “literal” );

See: http://stackoverflow.com/questions/4044255/passing-a-string-literal-to-a-function-that-takes-a-stdstring
//*/

const std::string Log::GlobalFilename( "logs/ScottEngine_Log.txt" );

Log::Log( void ) : Scope( "" ), LocalFile()
{
}

Log::Log( const std::string& LogFilename ) : Scope( LogFilename ), LocalFile( "logs/" + LogFilename + ".txt", std::ios_base::out )
{
	if( LocalFile )
	{
		LocalFile << Scope << " " << GetTimestamp() << "\n";
	}
	else
	{
		throw std::runtime_error( "Log: " + LogFilename + " log file stream creation failed" );
	}
}

Log::~Log( void )
{
	Message( Scope + " log stream closing" );
}

void Log::Open( const std::string& LogFilename )
{
	if( !LocalFile )
	{
		LocalFile.open( "logs/" + LogFilename + ".txt", std::ios_base::out );

		if( LocalFile )
		{
			LocalFile << Scope << " " << GetTimestamp() << "\n";
		}
		else
		{
			throw std::runtime_error( "Log: " + LogFilename + " log file stream creation failed" );
		}
	}
}

void Log::Message( const std::string& String, const bool StdOutput, const bool WriteToFile )
{
	if( StdOutput )
	{
		StandardOutput( String );
	}

	if( WriteToFile )
	{
		WriteToLogFile( String );
	}
}

void Log::operator()( const std::string& String, const bool StdOutput, const bool WriteToFile )
{
	if( StdOutput )
	{
		StandardOutput( String );
	}

	if( WriteToFile )
	{
		WriteToLogFile( String );
	}
}

void Log::StandardOutput( const std::string& String )
{
	std::cout << Scope << ": " << String << "\t" << GetTimestamp() << "\n";
}

void Log::WriteToLogFile( const std::string& String )
{
	if( LocalFile )
	{
		LocalFile << Scope << ": " << String << "\t" << GetTimestamp() << "\n";
	}
}

std::string Log::GetTimestamp( void )
{
	auto now = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
	std::tm* timeObj = std::localtime( &now );

	int year 	= ( timeObj->tm_year + 1900 );
	int month 	= ( timeObj->tm_mon + 1 );
	int day 	= timeObj->tm_mday;
	int hour 	= timeObj->tm_hour;
	int min 	= timeObj->tm_min;
	int sec 	= timeObj->tm_sec;

	std::ostringstream timeStamp;

	timeStamp << year << "-";

	if( month < 10 )
	{
		timeStamp << "0";
	}

	timeStamp << month << "-";

	if( day < 10 )
	{
		timeStamp << "0";
	}

	timeStamp << day << " ";

	if( hour < 10 )
	{
		timeStamp << "0";
	}

	timeStamp << hour << ":";

	if( min < 10 )
	{
		timeStamp << "0";
	}

	timeStamp << min << ":";

	if( sec < 10 )
	{
		timeStamp << "0";
	}

	timeStamp << sec;

	return timeStamp.str();
}