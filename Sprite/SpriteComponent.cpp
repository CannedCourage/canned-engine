#include "Sprite/SpriteComponent.h"

#include <d3dx9.h>

SpriteComponent::SpriteComponent( void )
{
	TextureDimensions.left = 0.0f;
	TextureDimensions.top = 0.0f;
	TextureDimensions.right = 1.0f;
	TextureDimensions.bottom = 1.0f;
}