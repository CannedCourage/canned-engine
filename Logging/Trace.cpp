#include "Logging/Trace.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

/*
If a method/function expects a const reference to string, it will also accept a string literal, e.g.
 
void Method( const std::string& str );
a.Method( “literal” );

See: http://stackoverflow.com/questions/4044255/passing-a-string-literal-to-a-function-that-takes-a-stdstring
//*/

//TODO: Add support for format strings
void Trace( const std::string& File, unsigned int Line, const std::string& Msg )
{
	std::string msg = File + "(" + std::to_string(Line) + "): " + Msg + "\n";

	std::cout << msg;
#ifdef _WIN32
	OutputDebugString( msg.c_str() );
#endif
}