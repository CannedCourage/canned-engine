#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3d9.h>
#include <d3dx9.h>

class System;
class Graphics;

class Texture
{
private:
protected:

	Graphics& graphics;
	LPDIRECT3DTEXTURE9 texture;
public:

	Texture( System &s );
	~Texture( void );

	LPDIRECT3DTEXTURE9* getTexture( void );

	void LoadFromFile( const char* file );
	void Unload( void );
};

#endif _TEXTURE_H_