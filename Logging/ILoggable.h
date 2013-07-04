#ifndef _ILOGGABLE_H_
#define _ILOGGABLE_H_

#include "Log.h"

class ILoggable
{
private:
protected:

	Log log;
public:

	ILoggable( const char* name = "SceneManager" ) : log( name )
	{
	}
	~ILoggable( void )
	{
	}
};

#endif //_ILOGGABLE_H_