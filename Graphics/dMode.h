#ifndef _DMODE_H_
#define _DMODE_H_

struct dMode
{
	unsigned int xRes, yRes, refresh, index;

	dMode( int x = 0, int y = 0, int r = 0, int i = 0 ) : xRes( x ), yRes( y ), refresh( r ), index( i )
	{
	}
};

#endif //_DMODE_H_