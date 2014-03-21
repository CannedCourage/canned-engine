#ifndef _MESHDATA_H_
#define _MESHDATA_H_

#include <d3d9.h>
#include <vector>

struct MeshData
{
	int vertexCount;
	int vertexSize;

	int indexSize;

	int triangleCount;
	D3DFORMAT indexFormat;

	std::vector<BYTE> vertexData;
	std::vector<BYTE> indexData;
	std::vector<D3DVERTEXELEMENT9> vertexElements;

	MeshData() : vertexCount(0), vertexSize(0), triangleCount(0), indexFormat(D3DFMT_UNKNOWN) {}
};

struct MeshComponentData
{
	int vertexCount;
	int vertexSize;

	int triangleCount;

	MeshComponentData() : vertexCount(0), vertexSize(0), triangleCount(0) {}
};

#endif //_MESHDATA_H_