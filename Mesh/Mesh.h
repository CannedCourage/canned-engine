#ifndef _MESH_H_
#define _MESH_H_

#include "Mesh\MeshData.h"

#include <d3dx9.h>

struct Mesh
{
	IDirect3DVertexBuffer9* vertexBuffer;
	IDirect3DVertexDeclaration9* vertexDeclaration;
	IDirect3DIndexBuffer9* indexBuffer;

	ID3DXEffect* effect;
	IDirect3DTexture9* diffuseMap;
	IDirect3DTexture9* normalMap;

	//int mDiffuseMapSamplerIndex[SAM_COUNT];
	//int mNormalMapSamplerIndex[SAM_COUNT];

	struct 
	{
		D3DXHANDLE viewProjection;
		D3DXHANDLE shaderTest;
		D3DXHANDLE boneTransforms;
		//D3DXHANDLE techniques[SAM_COUNT];
	} EffectParameters;

	MeshData data;

	Mesh() : vertexBuffer(NULL), vertexDeclaration(NULL), indexBuffer(NULL), effect(NULL), diffuseMap(NULL), normalMap(NULL)
	{
	}
};

#endif //_MESH_H_