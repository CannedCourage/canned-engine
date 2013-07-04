#include "Graphics\Texture.h"
#include "SceneManager\SceneManager.h"

Texture::Texture( void ) : graphics( &( SceneManager::Get()->graphics ) ), texture( NULL )
{
}

Texture::~Texture( void )
{
	Unload();
}

LPDIRECT3DTEXTURE9* Texture::getTexture( void )
{
	return &(texture);
}

void Texture::LoadFromFile( const char* file )
{
	Unload();

	graphics->ErrorCheck( D3DXCreateTextureFromFileA( graphics->Device(), file, &texture ), TEXT( "Creating Texture From File" ) );
}

void Texture::Unload( void )
{
	if( texture != NULL )
	{
		texture->Release();
		texture = NULL;
	}
}