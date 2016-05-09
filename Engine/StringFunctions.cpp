#include "StringFunctions.h"

using std::string;

string Piece( const string& Str, const string& Delimiter, int PieceIndex )
{
	if( PieceIndex < 0 )
	{
		return Str;
	}

	size_t posBeg = 0;
	size_t posEnd = 0;
	bool looped = false;
	string token;

	do
	{
		if( looped )
		{
			posBeg = posEnd + Delimiter.length();
		}

		if( ( posEnd = Str.find( Delimiter, posBeg ) ) == string::npos )
		{
			posEnd = ( Str.length() );
		}

		PieceIndex--;
		looped = true;
	}
	while( PieceIndex >= 0 );

	if( posBeg < 0 )
	{
		posBeg = 0;
	}

	token = Str.substr( posBeg, ( posEnd - posBeg ) );

	return token;
}

string StripPiece( string& Str, const string& Delimiter, int PieceIndex )
{
	if( PieceIndex < 0 )
	{
		return Str;
	}

	size_t posBeg = 0;
	size_t posEnd = 0;
	bool looped = false;
	string token;

	do
	{
		if( looped )
		{
			posBeg = posEnd + Delimiter.length();
		}

		if( ( posEnd = Str.find( Delimiter, posBeg ) ) == string::npos )
		{
			posEnd = ( Str.length() );
		}

		PieceIndex--;
		looped = true;
	}
	while( PieceIndex >= 0 );

	if( posBeg < 0 )
	{
		posBeg = 0;
	}

	token = Str.substr( posBeg, ( posEnd - posBeg ) );

    Str.erase( posBeg, ( posEnd - posBeg ) + Delimiter.length() );

    return token;
}