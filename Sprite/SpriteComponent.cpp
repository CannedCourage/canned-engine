#include "Sprite/SpriteComponent.h"

SpriteComponent::SpriteComponent( void ) : texture( NULL )
{
	texRect.left = 0;
	texRect.top = 0;
	texRect.right = 0;
	texRect.bottom = 0;
}