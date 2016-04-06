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

	IDirect3DVertexBuffer9* vBuffer = NULL;
	IDirect3DVertexDeclaration9* vDecl = NULL;
	IDirect3DIndexBuffer9* iBuffer = NULL;
	
	std::vector<VFormat> verts;
	std::vector<unsigned short int> indicies;
	std::vector<D3DVERTEXELEMENT9> vElements;
	
	const int numVerts = 4;
	const int posOffset = ( 0 * sizeof( float ) );
	const int texOffset = ( 3 * sizeof( float ) );
	const int stride = ( 5 * sizeof( float ) );

	ID3DXEffect* effect = NULL;
	const char* effectFile = "W:/engine/code/Sprite/SpriteShader.fx";
	DWORD shaderFlags = D3DXSHADER_USE_LEGACY_D3DX9_31_DLL;
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

	void AddSpriteComponent( const unsigned int entityID, const std::string& TextureAssetName );
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

	void operator()( const Entity& entity, const std::string& TextureAssetName );
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