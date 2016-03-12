#ifndef _SPRITEPROCESSOR_H_
#define _SPRITEPROCESSOR_H_

#include "Engine/iProcess.h"
#include "Sprite/SpriteComponent.h"
#include "Logging/Log.h"
#include <unordered_map>
#include <vector>
#include <d3dx9.h>

struct VFormat
{
	float x, y, z;
	float u, v;
};

class Graphics;
class TransformProcessor;

//TODO: Replace with vector?
typedef std::unordered_map<int, SpriteComponent> ListOfSprites;

class SpriteProcessor : public iProcess
{
private:
protected:

	Graphics& graphics;
	TransformProcessor& transforms;

	ListOfSprites spriteComponents; //entity id as key

	IDirect3DVertexBuffer9* vBuffer;
	IDirect3DVertexDeclaration9* vDecl;
	IDirect3DIndexBuffer9* iBuffer;
	
	std::vector<VFormat> verts;
	std::vector<unsigned short int> indicies;
	std::vector<D3DVERTEXELEMENT9> vElements;
	
	const int numVerts;
	const int posOffset;
	const int texOffset;
	const int stride;

	ID3DXEffect* effect;
	const char* effectFile;
	DWORD shaderFlags;
	D3DXHANDLE worldHandle, viewHandle, projHandle;
	D3DXHANDLE texTransHandle;
	D3DXHANDLE textureHandle;
	D3DXHANDLE RenderWithTexture;
	D3DXHANDLE RenderWithoutTexture;

	D3DXMATRIX viewMat, projMat;

	Log log;

	void DrawSprite( const unsigned int entityID, const SpriteComponent& sprite );
public:

	SpriteProcessor( Graphics& g, TransformProcessor& t );
	~SpriteProcessor( void );

	void AddSpriteComponent( const unsigned int entityID, IDirect3DTexture9* _texture );
	SpriteComponent& GetSpriteComponent( const unsigned int entityID );

	void Start( void );
	void Update( const EngineDuration& deltaT );
	void End( void );
};

class Entity;

class SpriteAdder
{
private:
protected:

	SpriteProcessor& sprites;
public:

	SpriteAdder( SpriteProcessor& s );

	void operator()( const Entity& entity, IDirect3DTexture9* texture );
};

class SpriteGetter
{
private:
protected:

	SpriteProcessor& sprites;
public:

	SpriteGetter( SpriteProcessor& s );

	SpriteComponent& operator()( const Entity& entity );
	const SpriteComponent& operator()( const Entity& entity ) const;
};

#endif //_SPRITEPROCESSOR_H_