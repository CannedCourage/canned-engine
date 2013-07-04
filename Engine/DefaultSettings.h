#ifndef _DEFAULTSETTINGS_H_
#define _DEFAULTSETTINGS_H_

#include "Logging\ILoggable.h"

#include <JsonBox.h>
using namespace JsonBox;

class DefaultSettings : ILoggable
{
private:
protected:

	Value in;

	Object root;
	Object::iterator objItr;

	Array arr;
	Array::iterator arrItr;

	Value out;

	const char* filename;
	const char* copy;
	const char* test;

	void ParseWindowDefaults( Value v );
	void ParseClientDefaults( Value v );
	void PrepareOutValue( void );
public:

	struct Window
	{
		//window
		int x, y, width, height;
		bool maximised;
	} window;

	struct Client
	{
		//client
		int x, y, xResolution, yResolution, width, height;
		bool fullscreen;
	} client;

	DefaultSettings( void );
	~DefaultSettings( void );

	void Save( void );
};

#endif //_DEFAULTSETTINGS_H_