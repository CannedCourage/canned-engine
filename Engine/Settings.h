#ifndef _DEFAULTSETTINGS_H_
#define _DEFAULTSETTINGS_H_

#include <JsonBox.h>
using namespace JsonBox;

#include <JSONValue.h>

class Log;

class Settings
{
private:
protected:

	Log log;

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

	Settings( void );
	~Settings( void );

	//New style settings methods
	void SetValue( char* settingName, int value );
	void SetValue( char* settingName, float value );
	void SetValue( char* settingName, char* value );
	int GetInt( void );
	float GetFloat( void );
	char* GetString( void );

	void Save( void );
};

#endif //_DEFAULTSETTINGS_H_