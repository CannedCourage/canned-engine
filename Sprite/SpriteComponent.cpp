#include "Sprite/SpriteComponent.h"

SpriteComponent::SpriteComponent( void ) : texture( NULL )
{
	texRect.left = 0.0f;
	texRect.top = 0.0f;
	texRect.right = 1.0f;
	texRect.bottom = 1.0f;

	rotation = 0.0f;
}