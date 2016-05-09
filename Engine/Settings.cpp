#include <iostream>
#include <stdexcept>

#include "Settings.h"
#include "StringFunctions.h"

using std::string;
using JsonBox::Value;
using JsonBox::Object;

Settings::Settings( const string& Filename, const string& Delimiter ) : Filename( Filename ), Delimiter( Delimiter )
{
	ReadFile();
}

Settings::~Settings( void )
{
	WriteFile();
}

void Settings::WriteFile( void )
{
	Value out( Root );

	out.writeToFile( Filename );
}

void Settings::ReadFile( void )
{
	Value in;

	in.loadFromFile( Filename );

	if( in.isObject() )
	{
		Root = in.getObject();
	}
	else
	{
		throw std::runtime_error{ "Opening JSON file failed: " + Filename };
	}
}

void Settings::SetDouble( const std::string& Name, const double& Val )
{
	Value v( Root );

	SetDoubleInObject( string( Name ), Val, v );

	Root = v.getObject();
}

void Settings::SetInteger( const std::string& Name, const int& Val )
{
	Value v( Root );

	SetIntegerInObject( string( Name ), Val, v );

	Root = v.getObject();
}

void Settings::SetString( const std::string& Name, const string& Val )
{
	Value v( Root );

	SetStringInObject( string( Name ), Val, v );

	Root = v.getObject();
}

void Settings::SetBool( const std::string& Name, const bool Val )
{
	Value v( Root );

	SetBoolInObject( string( Name ), Val, v );

	Root = v.getObject();
}

void Settings::SetDoubleInObject( string& Name, const double& Val, JsonBox::Value& JsonObject )
{
	Object obj = JsonObject.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( Name, Delimiter, 0 );

	//Get second substring
	second = Piece( Name, Delimiter, 0 );
	
	//Is there a second substring?
	if( second.length() == 0 )
	{
		//No
			//Create Value, initialise with value
			Value v = Value( Val );

			//Is there a value called first substring already?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Set object[ first substring ] equal to the new Value
					obj[ first.c_str() ] = v;
					JsonObject = Value( obj );
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
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
					}

				//Set object[ first substring ] equal to the new Value
				obj[ first.c_str() ] = v;
				JsonObject = obj;
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

					//Call SetDoubleInObject, pass Name, value and new object
					SetDoubleInObject( Name, Val, v );

					//Set object[ first substring ] equal to new object
					obj[ first.c_str() ] = v;
					JsonObject = obj;
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
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
					}

					//Call SetDoubleInObject, pass Name, value and object
					SetDoubleInObject( Name, Val, v );

					//Set object[ first substring ] equal to object
					obj[ first.c_str() ] = v;
					JsonObject = obj;
			}
	}
}

void Settings::SetIntegerInObject( string& Name, const int& Val, JsonBox::Value& JsonObject )
{
	Object obj = JsonObject.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( Name, Delimiter, 0 );

	//Get second substring
	second = Piece( Name, Delimiter, 0 );
	
	//Is there a second substring?
	if( second.length() == 0 )
	{
		//No
			//Create Value, initialise with value
			Value v = Value( Val );

			//Is there a value called first substring already?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Set object[ first substring ] equal to the new Value
					obj[ first.c_str() ] = v;
					JsonObject = Value( obj );
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
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
					}

				//Set object[ first substring ] equal to the new Value
				obj[ first.c_str() ] = v;
				JsonObject = obj;
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

					//Call SetIntegerInObject, pass Name, value and new object
					SetIntegerInObject( Name, Val, v );

					//Set object[ first substring ] equal to new object
					obj[ first.c_str() ] = v;
					JsonObject = obj;
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
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
					}

					//Call SetIntegerInObject, pass Name, value and object
					SetIntegerInObject( Name, Val, v );

					//Set object[ first substring ] equal to object
					obj[ first.c_str() ] = v;
					JsonObject = obj;
			}
	}
}

void Settings::SetStringInObject( string& Name, const string& Val, JsonBox::Value& JsonObject )
{
	Object obj = JsonObject.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( Name, Delimiter, 0 );

	//Get second substring
	second = Piece( Name, Delimiter, 0 );
	
	//Is there a second substring?
	if( second.length() == 0 )
	{
		//No
			//Create Value, initialise with value
			Value v = Value( Val );

			//Is there a value called first substring already?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Set object[ first substring ] equal to the new Value
					obj[ first.c_str() ] = v;
					JsonObject = Value( obj );
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
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
					}

				//Set object[ first substring ] equal to the new Value
				obj[ first.c_str() ] = v;
				JsonObject = obj;
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

					//Call SetStringInObject, pass Name, value and new object
					SetStringInObject( Name, Val, v );

					//Set object[ first substring ] equal to new object
					obj[ first.c_str() ] = v;
					JsonObject = obj;
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
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
					}

					//Call SetStringInObject, pass Name, value and object
					SetStringInObject( Name, Val, v );

					//Set object[ first substring ] equal to object
					obj[ first.c_str() ] = v;
					JsonObject = obj;
			}
	}
}

void Settings::SetBoolInObject( string& Name, const bool& Val, JsonBox::Value& JsonObject )
{
	Object obj = JsonObject.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( Name, Delimiter, 0 );

	//Get second substring
	second = Piece( Name, Delimiter, 0 );
	
	//Is there a second substring?
	if( second.length() == 0 )
	{
		//No
			//Create Value, initialise with value
			Value v = Value( Val );

			//Is there a value called first substring already?
			if( obj.find( first.c_str() ) == obj.end() )
			{
				//No
					//Set object[ first substring ] equal to the new Value
					obj[ first.c_str() ] = v;
					JsonObject = Value( obj );
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
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
					}

					//Set object[ first substring ] equal to the new Value
					obj[ first.c_str() ] = v;
					JsonObject = obj;
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

					//Call SetBoolInObject, pass Name, value and new object
					SetBoolInObject( Name, Val, v );

					//Set object[ first substring ] equal to new object
					obj[ first.c_str() ] = v;
					JsonObject = obj;
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
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
					}

					//Call SetBoolInObject, pass Name, value and object
					SetBoolInObject( Name, Val, v );

					//Set object[ first substring ] equal to object
					obj[ first.c_str() ] = v;
					JsonObject = obj;
			}
	}
}

double Settings::GetDouble( const std::string& Name )
{
	return GetDoubleFromObject( string( Name ), Root );
}

int Settings::GetInteger( const std::string& Name )
{
	return GetIntegerFromObject( string( Name ), Root );
}

string Settings::GetString( const std::string& Name )
{
	return GetStringFromObject( string( Name ), Root );
}

bool Settings::GetBool( const std::string& Name )
{
	return GetBoolFromObject( string( Name ), Root );
}
	
double Settings::GetDoubleFromObject( string& Name, const JsonBox::Value& JsonObject )
{
	Object obj = JsonObject.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( Name, Delimiter, 0 );

	//Get second substring
	second = Piece( Name, Delimiter, 0 );
	
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
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
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
							//Call GetDoubleFromObject, pass Name and object
							return GetDoubleFromObject( Name, v );
					}
					else
					{
						//No
							//Throw error, no sub object
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
					}
			}
	}

	return 0.0;
}

int Settings::GetIntegerFromObject( string& Name, const JsonBox::Value& JsonObject )
{
	Object obj = JsonObject.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( Name, Delimiter, 0 );

	//Get second substring
	second = Piece( Name, Delimiter, 0 );
	
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
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
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
							//Call GetIntegerFromObject, pass Name and object
							return GetIntegerFromObject( Name, v );
					}
					else
					{
						//No
							//Throw error, no sub object
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
					}
			}
	}

	return 0;
}

string Settings::GetStringFromObject( string& Name, const JsonBox::Value& JsonObject )
{
	Object obj = JsonObject.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( Name, Delimiter, 0 );

	//Get second substring
	second = Piece( Name, Delimiter, 0 );
	
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
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
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
							//Call GetStringFromObject, pass Name and object
							return GetStringFromObject( Name, v );
					}
					else
					{
						//No
							//Throw error, no sub object
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
					}
			}
	}

	return string( "" );
}

bool Settings::GetBoolFromObject( string& Name, const JsonBox::Value& JsonObject )
{
	Object obj = JsonObject.getObject();

	string first( "" );
	string second( "" );

	//Get first substring
	first = StripPiece( Name, Delimiter, 0 );

	//Get second substring
	second = Piece( Name, Delimiter, 0 );
	
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
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
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
							//Call GetBoolFromObject, pass Name and object
							return GetBoolFromObject( Name, v );
					}
					else
					{
						//No
							//Throw error, no sub object
						throw std::runtime_error{ "Error - Attempting to change setting data type: " + first };
					}
			}
	}

	return false;
}