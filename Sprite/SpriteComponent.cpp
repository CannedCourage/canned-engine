#include "Sprite/SpriteComponent.h"

#include "Graphics/Graphics.h"

SpriteComponent::SpriteComponent( void ) : texture( NULL )
{
	source.left = 0;
	source.top = 0;
	source.right = 0;
	source.bottom = 0;
}

SpriteComponent::SpriteComponent( IDirect3DTexture9* _texture ) : texture( _texture )
{
	source.left = 0;
	source.top = 0;
	source.right = 0;
	source.bottom = 0;
}

SpriteComponent::~SpriteComponent( void )
{
}

void SpriteComponent::SetSource( int left, int top, int right, int bottom )
{
	source.left = left;
	source.top = top;
	source.right = right;
	source.bottom = bottom;
}