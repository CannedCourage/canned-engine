#ifndef _LOG_H_
#define _LOG_H_

#include <iostream>
#include <fstream>
#include <string>

#ifdef _DEBUG
#define TRACE(expression) Trace(__FILE__,__LINE__,expression)
#else
#define TRACE __noop
#endif

void Trace( const std::string& File, unsigned int Line, const std::string& Msg );

class Log
{
private:
protected:

	const std::string Scope; 	//Where the message comes from
	std::ofstream LocalFile; 	//Log file stream

	static std::string GetTimestamp( void );
public:

	bool WriteToStdOutput = true;
	bool WriteToLogFile = true;
	bool WriteToDebugOutput = true;

	Log( void );
	Log( const std::string& LogFilename );
	~Log( void );

	void Open( const std::string& LogFilename );

	void Message( const std::string& String );
	void operator()( const std::string& String );

	template<typename T>
	Log& operator<<( const T& Output )
	{
		std::cout << Output << "\t" << GetTimestamp() << std::endl;
		LocalFile << Output << "\t" << GetTimestamp() << std::endl;

		return *this;
	}

	using streamFunction = std::ofstream& (*)( std::ostream& );

	Log& operator<<( streamFunction Function )
	{
		Function( std::cout );
		Function( LocalFile );

		return *this;
	}
};

#endif //_LOG_H_