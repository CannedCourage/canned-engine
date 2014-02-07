#include "StringFunctions.h"

using std::string;
using std::cout;
using std::endl;

string Piece( const string& str, const string& delimiter, int piece )
{
	if( piece < 0 )
	{
		return str;
	}

	size_t posBeg = 0;
	size_t posEnd = 0;
	bool looped = false;
	string token;

	do
	{
		if( looped )
		{
			posBeg = posEnd + delimiter.length();
		}

		if( ( posEnd = str.find( delimiter, posBeg ) ) == string::npos )
		{
			posEnd = ( str.length() );
		}

		piece--;
		looped = true;
	}
	while( piece >= 0 );

	if( posBeg < 0 )
	{
		posBeg = 0;
	}

	token = str.substr( posBeg, ( posEnd - posBeg ) );

	return token;
}

string StripPiece( string& str, const string& delimiter, int piece )
{
	if( piece < 0 )
	{
		return str;
	}

	size_t posBeg = 0;
	size_t posEnd = 0;
	bool looped = false;
	string token;

	do
	{
		if( looped )
		{
			posBeg = posEnd + delimiter.length();
		}

		if( ( posEnd = str.find( delimiter, posBeg ) ) == string::npos )
		{
			posEnd = ( str.length() );
		}

		piece--;
		looped = true;
	}
	while( piece >= 0 );

	if( posBeg < 0 )
	{
		posBeg = 0;
	}

	token = str.substr( posBeg, ( posEnd - posBeg ) );

    str.erase( posBeg, ( posEnd - posBeg ) + delimiter.length() );

    return token;
}