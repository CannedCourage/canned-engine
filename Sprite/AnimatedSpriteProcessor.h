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

	void DrawSprite( const unsigned int entityID, const AnimatedSpriteComponent& sprite );
public:

	AnimatedSpriteProcessor( Graphics& g, TransformProcessor& t );
	~AnimatedSpriteProcessor( void );

	void AddAnimatedSpriteComponent( const unsigned int entityID );
	AnimatedSpriteComponent& GetAnimatedSpriteComponent( const unsigned int entityID );

	Frame NewFrame( EngineDuration duration, const std::string& TextureAssetName );

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