#include "Engine/DefaultSettings.h"

#define SETTINGS "C:/Users/Scott/Programming/Projects/GitHub/canned-engine/Settings/DefaultSettings.json"
#define COPY "C:/Users/Scott/Programming/Projects/GitHub/canned-engine/Settings/copy.json"
#define TEST "C:/Users/Scott/Programming/Projects/GitHub/canned-engine/Settings/test.json"

DefaultSettings::DefaultSettings( void ) : log("DefaultSettings"), filename( SETTINGS ), copy( COPY ), test( TEST )
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
}

DefaultSettings::~DefaultSettings( void )
{
	Save(); //Save the defaults
}

void DefaultSettings::ParseWindowDefaults( Value v )
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

void DefaultSettings::ParseClientDefaults( Value v )
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

void DefaultSettings::PrepareOutValue( void )
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

void DefaultSettings::Save( void )
{
	PrepareOutValue();

	out.writeToFile( test );	//Preserve the results
	out.writeToFile( filename );	//Remember any changes
}