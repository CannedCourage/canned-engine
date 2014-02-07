#ifndef _STRINGFUNCS_H_
#define _STRINGFUNCS_H_

#include <iostream>

std::string Piece( const std::string& str, const std::string& delimiter, int piece = 0 );

std::string StripPiece( std::string& str, const std::string& delimiter, int piece = 0 );

#endif _STRINGFUNCS_H_