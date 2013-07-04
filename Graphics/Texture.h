#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Graphics.h"

class Texture
{
private:
protected:

	Graphics* const graphics;
	LPDIRECT3DTEXTURE9 texture;
public:

	Texture( void );
	~Texture( void );

	LPDIRECT3DTEXTURE9* getTexture( void );

	void LoadFromFile( const char* file );
	void Unload( void );
};

#endif _TEXTURE_H_