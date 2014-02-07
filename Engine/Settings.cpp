#include <iostream>

#include "Settings.h"
#include "StringFunctions.h"

using std::string;
using JsonBox::Value;
using JsonBox::Object;

Settings::Settings( const char* _filename, const char* _delimiter ) : filename( _filename ), root(), delimiter( _delimiter )
{
	ReadFile();
}

Settings::~Settings( void )
{
	WriteFile();
}

void Settings::WriteFile( void )
{
	Value out( root );

	//out.writeToStream( cout, true, true );

	out.writeToFile( filename );
}

void Settings::ReadFile( void )
{
	Value in;

	in.loadFromFile( filename );

	//string copy( filename );
	//copy.resize( copy.length() - 5 );
	//copy.append( "_COPY.json");
	
	string copy( "C:/Users/Scott/Programming/Projects/GitHub/canned-engine/Settings/MainSettings_COPY.json" );

	in.writeToFile( copy );	//Preserve the original

	//in.writeToStream( cout, true, true );

	if( in.isObject() )
	{
		root = in.getObject();
	}
	else
	{
		//Throw
	}
}

void Settings::SetDouble( string name, const double& value )
{
	Value v( root );

	SetDoubleInObject( name, value, v );

	root = v.getObject();
}

void Settings::SetInteger( string name, const int& value )
{
	Value v( root );

	SetIntegerInObject( name, value, v );

	root = v.getObject();
}

void Settings::SetString( string name, const string& value )
{
	Value v( root );

	SetStringInObject( name, value, v );

	root = v.getObject();
}

void Settings::SetBool( string name, const bool value )
{
	Value v( root );

	SetBoolInObject( name, value, v );

	root = v.getObject();
}

void Settings::SetDouble( const char* name, const double& value )
{
	Value v( root );

	SetDoubleInObject( string( name ), value, v );

	root = v.getObject();
}

void Settings::SetInteger( const char* name, const int& value )
{
	Value v( root );

	SetIntegerInObject( string( name ), value, v );

	root = v.getObject();
}

void Settings::SetString( const char* name, const string& value )
{
	Value v( root );

	SetStringInObject( string( name ), value, v );

	root = v.getObject();
}

void Settings::SetBool( const char* name, const bool value )
{
	Value v( root );

	SetBoolInObject( string( name ), value, v );

	root = v.getObject();
}

void Settings::SetDoubleInObject( string& name, const double& value, JsonBox::Value& object )
{
	Object obj = object.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( name, delimiter, 0 );

	//Get second substring
	second = Piece( name, delimiter, 0 );
	
	//Is there a second substring?
	if( second.length() == 0 )
	{
		//No
			//Create Value, initialise with value
			Value v = Value( value );

			//Is there a value called first substring already?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Set object[ first substring ] equal to the new Value
					obj[ first.c_str() ] = v;
					object = Value( obj );
			}
			else
			{
				//Yes
					//Is it a Double?
					if( obj[ first.c_str() ].isDouble() )
					{
						//Yes
							//Ok
					}
					else
					{
						//No
							//Throw error, don't change type
					}

				//Set object[ first substring ] equal to the new Value
				obj[ first.c_str() ] = v;
				object = obj;
			}
	}
	else
	{
		//Yes
			//Does object matching first substring exist?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Create new Object
					Object o;
					Value v( o );

					//Call SetDoubleInObject, pass name, value and new object
					SetDoubleInObject( name, value, v );

					//Set object[ first substring ] equal to new object
					obj[ first.c_str() ] = v;
					object = obj;
			}
			else
			{
				//Yes
					//Get Value
					Value v = obj[ first.c_str() ];

					//Is the value an object?
					if( v.isObject() )
					{
						//Yes
							//Ok
					}
					else
					{
						//No
							//Throw error, don't change type
					}

					//Call SetDoubleInObject, pass name, value and object
					SetDoubleInObject( name, value, v );

					//Set object[ first substring ] equal to object
					obj[ first.c_str() ] = v;
					object = obj;
			}
	}
}

void Settings::SetIntegerInObject( string& name, const int& value, JsonBox::Value& object )
{
	Object obj = object.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( name, delimiter, 0 );

	//Get second substring
	second = Piece( name, delimiter, 0 );
	
	//Is there a second substring?
	if( second.length() == 0 )
	{
		//No
			//Create Value, initialise with value
			Value v = Value( value );

			//Is there a value called first substring already?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Set object[ first substring ] equal to the new Value
					obj[ first.c_str() ] = v;
					object = Value( obj );
			}
			else
			{
				//Yes
					//Is it an Integer?
					if( obj[ first.c_str() ].isInteger() )
					{
						//Yes
							//Ok
					}
					else
					{
						//No
							//Throw error, don't change type
					}

				//Set object[ first substring ] equal to the new Value
				obj[ first.c_str() ] = v;
				object = obj;
			}
	}
	else
	{
		//Yes
			//Does object matching first substring exist?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Create new Object
					Object o;
					Value v( o );

					//Call SetIntegerInObject, pass name, value and new object
					SetIntegerInObject( name, value, v );

					//Set object[ first substring ] equal to new object
					obj[ first.c_str() ] = v;
					object = obj;
			}
			else
			{
				//Yes
					//Get Value
					Value v = obj[ first.c_str() ];

					//Is the value an object?
					if( v.isObject() )
					{
						//Yes
							//Ok
					}
					else
					{
						//No
							//Throw error, don't change type
					}

					//Call SetIntegerInObject, pass name, value and object
					SetIntegerInObject( name, value, v );

					//Set object[ first substring ] equal to object
					obj[ first.c_str() ] = v;
					object = obj;
			}
	}
}

void Settings::SetStringInObject( string& name, const string& value, JsonBox::Value& object )
{
	Object obj = object.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( name, delimiter, 0 );

	//Get second substring
	second = Piece( name, delimiter, 0 );
	
	//Is there a second substring?
	if( second.length() == 0 )
	{
		//No
			//Create Value, initialise with value
			Value v = Value( value );

			//Is there a value called first substring already?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Set object[ first substring ] equal to the new Value
					obj[ first.c_str() ] = v;
					object = Value( obj );
			}
			else
			{
				//Yes
					//Is it a string?
					if( obj[ first.c_str() ].isString() )
					{
						//Yes
							//Ok
					}
					else
					{
						//No
							//Throw error, don't change type
					}

				//Set object[ first substring ] equal to the new Value
				obj[ first.c_str() ] = v;
				object = obj;
			}
	}
	else
	{
		//Yes
			//Does object matching first substring exist?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Create new Object
					Object o;
					Value v( o );

					//Call SetStringInObject, pass name, value and new object
					SetStringInObject( name, value, v );

					//Set object[ first substring ] equal to new object
					obj[ first.c_str() ] = v;
					object = obj;
			}
			else
			{
				//Yes
					//Get Value
					Value v = obj[ first.c_str() ];

					//Is the value an object?
					if( v.isObject() )
					{
						//Yes
							//Ok
					}
					else
					{
						//No
							//Throw error, don't change type
					}

					//Call SetStringInObject, pass name, value and object
					SetStringInObject( name, value, v );

					//Set object[ first substring ] equal to object
					obj[ first.c_str() ] = v;
					object = obj;
			}
	}
}

void Settings::SetBoolInObject( string& name, const bool value, JsonBox::Value& object )
{
	Object obj = object.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( name, delimiter, 0 );

	//Get second substring
	second = Piece( name, delimiter, 0 );
	
	//Is there a second substring?
	if( second.length() == 0 )
	{
		//No
			//Create Value, initialise with value
			Value v = Value( value );

			//Is there a value called first substring already?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Set object[ first substring ] equal to the new Value
					obj[ first.c_str() ] = v;
					object = Value( obj );
			}
			else
			{
				//Yes
					//Is it a bool?
					if( obj[ first.c_str() ].isBoolean () )
					{
						//Yes
							//Ok
					}
					else
					{
						//No
							//Throw error, don't change type
					}

					//Set object[ first substring ] equal to the new Value
					obj[ first.c_str() ] = v;
					object = obj;
			}
	}
	else
	{
		//Yes
			//Does object matching first substring exist?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Create new Object
					Object o;
					Value v( o );

					//Call SetBoolInObject, pass name, value and new object
					SetBoolInObject( name, value, v );

					//Set object[ first substring ] equal to new object
					obj[ first.c_str() ] = v;
					object = obj;
			}
			else
			{
				//Yes
					//Get Value
					Value v = obj[ first.c_str() ];

					//Is the value an object?
					if( v.isObject() )
					{
						//Yes
							//Ok
					}
					else
					{
						//No
							//Throw error, don't change type
					}

					//Call SetBoolInObject, pass name, value and object
					SetBoolInObject( name, value, v );

					//Set object[ first substring ] equal to object
					obj[ first.c_str() ] = v;
					object = obj;
			}
	}
}

double Settings::GetDouble( string name )
{
	return GetDoubleFromObject( name, root );
}

int Settings::GetInteger( string name )
{
	return GetIntegerFromObject( name, root );
}

string Settings::GetString( string name )
{
	return GetStringFromObject( name, root );
}

bool Settings::GetBool( string name )
{
	return GetBoolFromObject( name, root );
}

double Settings::GetDouble( const char* name )
{
	return GetDoubleFromObject( string( name ), root );
}

int Settings::GetInteger( const char* name )
{
	return GetIntegerFromObject( string( name ), root );
}

string Settings::GetString( const char* name )
{
	return GetStringFromObject( string( name ), root );
}

bool Settings::GetBool( const char* name )
{
	return GetBoolFromObject( string ( name ), root );
}
	
double Settings::GetDoubleFromObject( string& name, const JsonBox::Value& object )
{
	Object obj = object.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( name, delimiter, 0 );

	//Get second substring
	second = Piece( name, delimiter, 0 );
	
	//Is there a second substring?
	if( second.length() == 0 )
	{
		//No
			//Is there a value called <first substring> already?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Throw error, no such setting exists
			}
			else
			{
				Value v = obj.find( first.c_str() )->second;
				//Yes
					//Is it a Double?
					if( v.isDouble() )
					{
						//Yes
							//Get Double from object [ first substring ]
							return v.getDouble();
					}
					else
					{
						//No
							//Throw error, don't change type
					}
			}
	}
	else
	{
		//Yes
			//Does value matching first substring exist?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Throw error, no such setting exists
			}
			else
			{
				//Yes
					//Get Value
					Value v = obj.find( first.c_str() )->second;

					//Is the value an object?
					if( v.isObject() )
					{
						//Yes
							//Call GetDoubleFromObject, pass name and object
							return GetDoubleFromObject( name, v );
					}
					else
					{
						//No
							//Throw error, no sub object
					}
			}
	}

	return 0.0;
}

int Settings::GetIntegerFromObject( string& name, const JsonBox::Value& object )
{
	Object obj = object.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( name, delimiter, 0 );

	//Get second substring
	second = Piece( name, delimiter, 0 );
	
	//Is there a second substring?
	if( second.length() == 0 )
	{
		//No
			//Is there a value called <first substring> already?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Throw error, no such setting exists
			}
			else
			{
				Value v = obj.find( first.c_str() )->second;
				//Yes
					//Is it a Integer?
					if( v.isInteger() )
					{
						//Yes
							//Get Integer from object [ first substring ]
							return v.getInt();
					}
					else
					{
						//No
							//Throw error, don't change type
					}
			}
	}
	else
	{
		//Yes
			//Does value matching first substring exist?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Throw error, no such setting exists
			}
			else
			{
				//Yes
					//Get Value
					Value v = obj.find( first.c_str() )->second;

					//Is the value an object?
					if( v.isObject() )
					{
						//Yes
							//Call GetIntegerFromObject, pass name and object
							return GetIntegerFromObject( name, v );
					}
					else
					{
						//No
							//Throw error, no sub object
					}
			}
	}

	return 0;
}

string Settings::GetStringFromObject( string& name, const JsonBox::Value& object )
{
	Object obj = object.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( name, delimiter, 0 );

	//Get second substring
	second = Piece( name, delimiter, 0 );
	
	//Is there a second substring?
	if( second.length() == 0 )
	{
		//No
			//Is there a value called <first substring> already?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Throw error, no such setting exists
			}
			else
			{
				Value v = obj.find( first.c_str() )->second;
				//Yes
					//Is it a string?
					if( v.isString() )
					{
						//Yes
							//Get string from object [ first substring ]
							return v.getString();
					}
					else
					{
						//No
							//Throw error, don't change type
					}
			}
	}
	else
	{
		//Yes
			//Does value matching first substring exist?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Throw error, no such setting exists
			}
			else
			{
				//Yes
					//Get Value
					Value v = obj.find( first.c_str() )->second;

					//Is the value an object?
					if( v.isObject() )
					{
						//Yes
							//Call GetStringFromObject, pass name and object
							return GetStringFromObject( name, v );
					}
					else
					{
						//No
							//Throw error, no sub object
					}
			}
	}

	return string( "" );
}

bool Settings::GetBoolFromObject( string& name, const JsonBox::Value& object )
{
	Object obj = object.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( name, delimiter, 0 );

	//Get second substring
	second = Piece( name, delimiter, 0 );
	
	//Is there a second substring?
	if( second.length() == 0 )
	{
		//No
			//Is there a value called <first substring> already?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Throw error, no such setting exists
			}
			else
			{
				Value v = obj.find( first.c_str() )->second;
				//Yes
					//Is it a bool?
					if( v.isBoolean() )
					{
						//Yes
							//Get bool from object [ first substring ]
							return v.getBoolean();
					}
					else
					{
						//No
							//Throw error, don't change type
					}
			}
	}
	else
	{
		//Yes
			//Does value matching first substring exist?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Throw error, no such setting exists
			}
			else
			{
				//Yes
					//Get Value
					Value v = obj.find( first.c_str() )->second;

					//Is the value an object?
					if( v.isObject() )
					{
						//Yes
							//Call GetBoolFromObject, pass name and object
							return GetBoolFromObject( name, v );
					}
					else
					{
						//No
							//Throw error, no sub object
					}
			}
	}

	return false;
}