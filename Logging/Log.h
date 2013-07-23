#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>

#define DEFAULT_LOG_FILENAME "ScottEngine_Log"

class Log
{
private:
protected:

	static const char* globalFilename;	//Master log filename
	static FILE* globalFile;			//Master log file
	static int counter;

	const char* scope;				//The original scope the message comes from
	FILE* localFile;				//File local to the log object

	time_t rawTime;
	struct tm* time_GMT;

	void Show( const char* s );
	void Print( const char* s );

	void UpdateTime( void );
public:

	Log( void );
	Log( const char* scope_ );
	~Log( void );

	void Message( const char* s, const bool show = false, const bool print = true );
};

#endif //_LOG_H_