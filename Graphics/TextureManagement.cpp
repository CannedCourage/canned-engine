#include "Graphics/Graphics.h"

void Graphics::LoadTexture( const std::string& Filename, const std::string& AssetName )
{
	auto result = Textures.find( AssetName );

	if( result != Textures.end() )
	{
		return;
	}

	LPDIRECT3DTEXTURE9 tex = NULL;
	
	ErrorCheck( D3DXCreateTextureFromFile( Device(), Filename.c_str(), &tex ), "Creating Texture: " + Filename );
	
	if( tex != NULL )
	{
		Textures[AssetName] = tex;
	}
}

IDirect3DTexture9* Graphics::GetTexture( const std::string& AssetName )
{
	return Textures.at( AssetName );
}

void Graphics::ReleaseTextures( void )
{
	ListOfTextureAssets::iterator it;

	for( it = Textures.begin(); it != Textures.end(); it++ )
	{
		if( it->second )
		{
			ErrorCheck( it->second->Release(), "Releasing Textures" );

			it->second = NULL; //Always null your pointers
		}
	}

	Textures.clear();
}