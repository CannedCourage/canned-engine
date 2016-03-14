#ifndef _ANIMATEDSPRITEPROCESSOR_H_
#define _ANIMATEDSPRITEPROCESSOR_H_

#include "Engine/iProcess.h"
#include "Sprite/AnimatedSpriteComponent.h"
#include "Logging/Log.h"
#include <unordered_map>
#include <vector>
#include <d3dx9.h>

#include "Sprite/SpriteProcessor.h"

class Graphics;
class TransformProcessor;

//TODO: Replace with vector?
typedef std::unordered_map<int, AnimatedSpriteComponent> ListOfAnimatedSprites;

class AnimatedSpriteProcessor : public iProcess
{
private:
protected:

	Graphics& graphics;
	TransformProcessor& transforms;

	ListOfAnimatedSprites spriteComponents; //entity id as key

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

	void DrawSprite( const unsigned int entityID, const AnimatedSpriteComponent& sprite );
public:

	AnimatedSpriteProcessor( Graphics& g, TransformProcessor& t );
	~AnimatedSpriteProcessor( void );

	void AddAnimatedSpriteComponent( const unsigned int entityID );
	AnimatedSpriteComponent& GetAnimatedSpriteComponent( const unsigned int entityID );

	void Start( void );
	void Update( const EngineDuration& deltaT );
	void End( void );
};

class Entity;

class AnimatedSpriteAdder
{
private:
protected:

	AnimatedSpriteProcessor& sprites;
public:

	AnimatedSpriteAdder( AnimatedSpriteProcessor& s );

	void operator()( const Entity& entity );
};

class AnimatedSpriteGetter
{
private:
protected:

	AnimatedSpriteProcessor& sprites;
public:

	AnimatedSpriteGetter( AnimatedSpriteProcessor& s );

	AnimatedSpriteComponent& operator()( const Entity& entity );
	const AnimatedSpriteComponent& operator()( const Entity& entity ) const;
};

#endif //_ANIMATEDSPRITEPROCESSOR_H_