#ifndef _ADAPTER_H_
#define _ADAPTER_H_

struct adapterDesc
{
	char* desc;
	int index;

	adapterDesc( char* d = "", int i = 0 ) : desc( d ), index( i )
	{
	}
};

#endif //_ADAPTER_H_