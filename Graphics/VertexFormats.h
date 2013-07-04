#ifndef _VERTEX_FORMATS_H_
#define _VERTEX_FORMATS_H_

#include <d3d9.h>

struct Vertex2d
{
	float x, y, z, rhw;
	static const DWORD format = D3DFVF_XYZRHW;
	static int stride;
};

struct Vertex3d
{
	float x, y, z;
	static const DWORD format = D3DFVF_XYZ;
	static int stride;
};

struct Vertex2dDc
{
	float x, y, z, rhw;
    DWORD colour;
	static const DWORD format = ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	static int stride;
};

struct Vertex3dDc
{
	float x, y, z;
	DWORD colour;
	static const DWORD format = ( D3DFVF_XYZ | D3DFVF_DIFFUSE );
	static int stride;
};

#define FVF3DDC	( D3DFVF_XYZ | D3DFVF_DIFFUSE )

struct test
{
	float x, y, z;
	DWORD colour;
};

struct Vertex3dTx
{
    float x, y, z;
    float tu, tv;
	static const DWORD format = ( D3DFVF_XYZ | D3DFVF_TEX1 );
	static int stride;
};

struct Vertex2dTx
{
    float x, y, z, rhw;
    float tu, tv;
	static const DWORD format = ( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	static int stride;
};

#endif //_VERTEX_FORMATS_H_