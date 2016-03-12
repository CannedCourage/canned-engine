#include "Sprite/SpriteComponent.h"

#include <d3dx9.h>

SpriteComponent::SpriteComponent( void ) : Texture( NULL )
{
	TextureDimensions.left = 0.0f;
	TextureDimensions.top = 0.0f;
	TextureDimensions.right = 1.0f;
	TextureDimensions.bottom = 1.0f;

	TextureRotation = 0.0f;
}