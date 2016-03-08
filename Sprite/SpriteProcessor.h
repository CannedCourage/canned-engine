#ifndef _SPRITEPROCESSOR_H_
#define _SPRITEPROCESSOR_H_

#include "Engine/iProcess.h"

#include "Sprite/SpriteComponent.h"

#include "Logging/Log.h"

#include <map>
#include <vector>

//#include "Entity/Entity.h"

struct VFormat
{
	float x, y, z;
	float u, v;
};

class Graphics;
class TransformProcessor;

class SpriteProcessor : public iProcess
{
private:
protected:

	Graphics& graphics;
	TransformProcessor& transforms;

	//TODO: Replace with std::vector< std::pair< int, SpriteComponent > > ?
	std::map<int, SpriteComponent> spriteComponents; //entity id as key

	IDirect3DVertexBuffer9* vBuffer;
	IDirect3DVertexDeclaration9* vDecl;
	IDirect3DIndexBuffer9* iBuffer;
	
	std::vector<VFormat> verts;
	std::vector<short> indicies;
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

	Log log;
public:

	SpriteProcessor( Graphics& g, TransformProcessor& t );
	~SpriteProcessor( void );

	void AddSpriteComponent( const unsigned int entityID, IDirect3DTexture9* _texture );
	SpriteComponent& GetSpriteComponent( const unsigned int entityID );

	void Start( void );
	void Update( const double& deltaT );
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