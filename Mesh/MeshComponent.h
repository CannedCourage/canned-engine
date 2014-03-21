#ifndef _MESHCOMPONENT_H_
#define _MESHCOMPONENT_H_

#include <d3dx9.h>

#include "Mesh/Mesh.h"

struct MeshComponent
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

	MeshComponentData data;

	MeshComponent() : vertexBuffer(NULL), vertexDeclaration(NULL), indexBuffer(NULL), effect(NULL), diffuseMap(NULL), normalMap(NULL)
	{
	}

	MeshComponent& operator=( Mesh& rhs )
	{
		this->vertexBuffer = rhs.vertexBuffer;
		this->vertexDeclaration = rhs.vertexDeclaration;
		this->indexBuffer = rhs.indexBuffer;
		this->effect = rhs.effect;
		this->diffuseMap = rhs.diffuseMap;
		this->normalMap = rhs.normalMap;

		this->EffectParameters.viewProjection = rhs.EffectParameters.viewProjection;
		this->EffectParameters.shaderTest = rhs.EffectParameters.shaderTest;
		this->EffectParameters.boneTransforms = rhs.EffectParameters.boneTransforms;

		this->data.vertexCount = rhs.data.vertexCount;
		this->data.vertexSize = rhs.data.vertexSize;
		this->data.triangleCount = rhs.data.triangleCount;
	}
};

#endif //_MESHCOMPONENT_H_