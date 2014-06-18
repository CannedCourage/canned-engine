#include "Sprite/SpriteProcessor.h"
#include "Maths/VectorToD3D.h"
#include "Graphics/Graphics.h"

#include <assert.h>

SpriteProcessor::SpriteProcessor( Graphics& g ) : graphics( g ), spriteInterface( NULL ), log( "SpriteProcessor" )
{
	graphics.ErrorCheck( D3DXCreateSprite( graphics.Device(), &spriteInterface ), "Creating Sprite Interface" );
}

SpriteProcessor::~SpriteProcessor( void )
{
	if( spriteInterface != NULL )
	{
		spriteInterface->Release();
		spriteInterface = NULL;
	}
}

void SpriteProcessor::AddSpriteComponent( const unsigned int entityID, IDirect3DTexture9* texture )
{
	SpriteComponent s( texture );

	spriteComponents[ entityID ] = s;
}

SpriteComponent& SpriteProcessor::GetSpriteComponent( const unsigned int entityID )
{
	return spriteComponents[ entityID ];
}

void SpriteProcessor::Start( void )
{
}

void SpriteProcessor::Update( const double& deltaT )
{
	std::map<int, SpriteComponent>::iterator it;

	spriteInterface->Begin( D3DXSPRITE_ALPHABLEND );

	for( it = spriteComponents.begin(); it != spriteComponents.end(); it++ )
	{
		IDirect3DTexture9* t = it->second.texture;
		D3DCOLOR c = D3DCOLOR_RGBA( 1, 1, 1, 1 );

		D3DXVECTOR3 cen = VectorToD3D(it->second.center);
		D3DXVECTOR3 tran = VectorToD3D(it->second.translation);
		RECT source = (it->second.source);

		//(*it)->sprite->Draw( (*it)->texture, &(*it)->source, &(*it)->center.ToD3D(), &(*it)->position.ToD3D(), D3DCOLOR( 1.0f, 1.0f, 1.0f ) );
		//graphics.ErrorCheck( spriteInterface->Draw( t, &source, &cen, &tran, c ), "Drawing Sprite" );
		graphics.ErrorCheck( spriteInterface->Draw( t, NULL, NULL, NULL, c ), "Drawing Sprite" );
	}

	spriteInterface->End();
}

void SpriteProcessor::End( void )
{
}