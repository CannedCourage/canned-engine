#ifndef _STRINGFUNCTIONS_H_
#define _STRINGFUNCTIONS_H_

#include <iostream>

std::string Piece( const std::string& Str, const std::string& Delimiter, int PieceIndex = 0 );

std::string StripPiece( std::string& Str, const std::string& Delimiter, int PieceIndex = 0 );

#endif _STRINGFUNCTIONS_H_