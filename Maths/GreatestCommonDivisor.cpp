#include "Maths/GreatestCommonDivisor.h"

int GCD( const int& a, const int& b )
{
    return ( b == 0 ) ? a : GCD( b, a % b );
}