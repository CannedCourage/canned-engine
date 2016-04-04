#ifndef _LOG_H_
#define _LOG_H_

#include <fstream>
#include <string>

class Log
{
private:
protected:

	static const std::string GlobalFilename; 	//Master log filename

	const std::string Scope; 	//Where the message comes from
	std::ofstream LocalFile; 	//Log file stream

	void StandardOutput( const std::string& String );
	void WriteToLogFile( const std::string& String );

	std::string GetTimestamp( void );
public:

	Log( void );
	Log( const std::string& LogFilename );
	~Log( void );

	void Open( const std::string& LogFilename );

	void Message( const std::string& String, const bool StdOutput = false, const bool WriteToFile = true );
	void operator()( const std::string& String, const bool StdOutput = false, const bool WriteToFile = true );
};

#endif //_LOG_H_