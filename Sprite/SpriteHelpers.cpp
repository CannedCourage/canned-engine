#include "Sprite/SpriteProcessor.h"
#include "Entity/Entity.h"

//ADD COMPONENT//
SpriteAdder::SpriteAdder( SpriteProcessor& s ) : sprites( s )
{
}

void SpriteAdder::operator()( const Entity& entity, const std::string& TextureAssetName )
{
	sprites.AddSpriteComponent( entity.ID, TextureAssetName );
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