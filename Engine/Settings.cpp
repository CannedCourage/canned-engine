#include "Logging/Log.h"
#include "Engine/Settings.h"

#include <iostream>
#include <fstream>

using namespace std;

#define SETTINGS 	"C:/Users/Scott/Programming/Projects/Scott_Oliver_Engine/Scott_Oliver_Engine/Settings/DefaultSettings.json"
#define COPY 		"C:/Users/Scott/Programming/Projects/Scott_Oliver_Engine/Scott_Oliver_Engine/Settings/copy.json"
#define TEST 		"C:/Users/Scott/Programming/Projects/Scott_Oliver_Engine/Scott_Oliver_Engine/Settings/test.json"

Settings::Settings( void ) : log("Settings"), filename( SETTINGS ), copy( COPY ), test( TEST )
{
	in.loadFromFile( filename );
	in.writeToFile( copy );	//Preserve the original

	root = in.getObject();

	for( objItr = root.begin(); objItr != root.end(); objItr++ )
	{
		log.Message( objItr->first.data() );
		
		if( strcmp( objItr->first.data(), "window" ) == 0 )
		{
			ParseWindowDefaults( objItr->second );
		}

		if( strcmp( objItr->first.data(), "client" ) == 0 )
		{
			ParseClientDefaults( objItr->second );
		}
	}
/*
const wchar_t* EXAMPLE = L"\
{ \
	\"string_name\" : \"string\tvalue and a \\\"quote\\\" and a unicode char \\u00BE and a c:\\\\path\\\\ or a \\/unix\\/path\\/ :D\", \
	\"bool_name\" : true, \
	\"bool_second\" : FaLsE, \
	\"null_name\" : nULl, \
	\"negative\" : -34.276, \
	\"sub_object\" : { \
						\"foo\" : \"abc\", \
						 \"bar\" : 1.35e2, \
						 \"blah\" : { \"a\" : \"A\", \"b\" : \"B\", \"c\" : \"C\" } \
					}, \
	\"array_letters\" : [ \"a\", \"b\", \"c\", [ 1, 2, 3  ]  ] \
}    ";

	JSONValue *value = JSON::Parse(EXAMPLE);

	JSONObject root;

	root = value->AsObject();

	if ( root.find(L"client") != root.end() && root[L"client"]->IsString() )
	{
		log.Message( "client is object.", true );
		//print_out(root[L"client"]->AsString().c_str());
		//print_out(L"\r\n\r\n");
	}
	//*/
}

Settings::~Settings( void )
{
	Save(); //Save the defaults
}

void Settings::ParseWindowDefaults( Value v )
{
	Object _window;
	Object::iterator itr;

	if( v.isObject() )
	{
		_window = v.getObject();

		for( itr = _window.begin(); itr != _window.end(); itr++ )
		{
			if( strcmp( itr->first.data(), "x" ) == 0 )
			{
				window.x = itr->second.getInt();
			}
			if( strcmp( itr->first.data(), "y" ) == 0 )
			{
				window.y = itr->second.getInt();
			}
			if( strcmp( itr->first.data(), "width" ) == 0 )
			{
				window.width = itr->second.getInt();
			}
			if( strcmp( itr->first.data(), "height" ) == 0 )
			{
				window.height = itr->second.getInt();
			}
			if( strcmp( itr->first.data(), "maximised" ) == 0 )
			{
				window.maximised = itr->second.getBoolean();
			}
		}
	}
	else
	{
		log.Message( "window value is not object", true );
	}
}

void Settings::ParseClientDefaults( Value v )
{
	Object _client;
	Object::iterator itr;

	if( v.isObject() )
	{
		_client = v.getObject();

		for( itr = _client.begin(); itr != _client.end(); itr++ )
		{
			if( strcmp( itr->first.data(), "xResolution" ) == 0 )
			{
				client.xResolution = itr->second.getInt();
			}
			if( strcmp( itr->first.data(), "yResolution" ) == 0 )
			{
				client.yResolution = itr->second.getInt();
			}
			if( strcmp( itr->first.data(), "fullscreen" ) == 0 )
			{
				client.fullscreen = itr->second.getBoolean();
			}
			if( strcmp( itr->first.data(), "x" ) == 0 )
			{
				client.x = itr->second.getInt();
			}
			if( strcmp( itr->first.data(), "y" ) == 0 )
			{
				client.y = itr->second.getInt();
			}
			if( strcmp( itr->first.data(), "width" ) == 0 )
			{
				client.width = itr->second.getInt();
			}
			if( strcmp( itr->first.data(), "height" ) == 0 )
			{
				client.height = itr->second.getInt();
			}
		}
	}
	else
	{
		log.Message( "client value is not object", true );
	}
}

void Settings::PrepareOutValue( void )
{
	out["window"]["x"] = Value( window.x );
	out["window"]["y"] = Value( window.y );
	out["window"]["width"] = Value( window.width );
	out["window"]["height"] = Value( window.height );
	out["window"]["maximised"] = Value( window.maximised );

	out["client"]["xResolution"] = Value( client.xResolution );
	out["client"]["yResolution"] = Value( client.yResolution );
	out["client"]["fullscreen"] = Value( client.fullscreen );
	out["client"]["x"] = Value( client.x );
	out["client"]["y"] = Value( client.y );
	out["client"]["width"] = Value( client.width );
	out["client"]["height"] = Value( client.height );
}

void Settings::Save( void )
{
	PrepareOutValue();

	out.writeToFile( test );	//Preserve the results
	out.writeToFile( filename );	//Remember any changes
}

void SetValue( char* settingName, int value )
{
	//Parse settingName
	//
	//Add value
}

void SetValue( char* settingName, float value )
{

}

void SetValue( char* settingName, char* value )
{

}

int GetInt( void )
{
	return 0;
}

float GetFloat( void )
{
	return 0.0f;
}

char* GetString( void )
{
	return (char*)0;
}