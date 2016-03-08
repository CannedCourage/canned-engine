#include "Sprite/SpriteProcessor.h"

#include "Entity/Entity.h"

//ADD COMPONENT//
SpriteAdder::SpriteAdder( SpriteProcessor& s ) : sprites( s )
{
}

void SpriteAdder::operator()( const Entity& entity, IDirect3DTexture9* texture )
{
	sprites.AddSpriteComponent( entity.ID, texture );
}

//GET COMPONENT//
SpriteGetter::SpriteGetter( SpriteProcessor& s ) : sprites( s )
{
}

SpriteComponent& SpriteGetter::operator()( const Entity& entity )
{
	return sprites.GetSpriteComponent( entity.ID );
}

const SpriteComponent& SpriteGetter::operator()( const Entity& entity ) const
{
	return sprites.GetSpriteComponent( entity.ID );
}