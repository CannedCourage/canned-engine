#include "Assets/AssetManager.h"
#include "Graphics/Graphics.h"

void AssetManager::LoadTexture( const char* filename )
{
	LPDIRECT3DTEXTURE9 tex = NULL;
	std::string name( filename );
	
	graphics.ErrorCheck( D3DXCreateTextureFromFile( graphics.Device(), filename, &tex ), "Creating Texture" );
	
	if( tex != NULL )
	{
		textureAssets[ name ] = tex;
	}
}

void AssetManager::ReleaseTextures( void )
{
	std::map<std::string, IDirect3DTexture9*>::iterator it;

	for(it = textureAssets.begin(); it != textureAssets.end(); it++)
	{
		if( it->second )
		{
			graphics.ErrorCheck( it->second->Release(), "Releasing Textures" );

			it->second = NULL; //Always null your pointers
		}
	}
}

IDirect3DTexture9* AssetManager::GetTexture( const char* filename )
{
	std::string name( filename );
	
	return textureAssets[ name ];
}