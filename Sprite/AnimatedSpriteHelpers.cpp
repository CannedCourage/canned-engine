/*
#include "Sprite/AnimatedSpriteProcessor.h"
#include "Entity/Entity.h"

//ADD COMPONENT//
AnimatedSpriteAdder::AnimatedSpriteAdder( AnimatedSpriteProcessor& s ) : sprites( s )
{
}

void AnimatedSpriteAdder::operator()( const Entity& entity )
{
	sprites.AddAnimatedSpriteComponent( entity.ID );
}

//GET COMPONENT//
AnimatedSpriteGetter::AnimatedSpriteGetter( AnimatedSpriteProcessor& s ) : sprites( s )
{
}

AnimatedSpriteComponent& AnimatedSpriteGetter::operator()( const Entity& entity )
{
	return sprites.GetAnimatedSpriteComponent( entity.ID );
}

const AnimatedSpriteComponent& AnimatedSpriteGetter::operator()( const Entity& entity ) const
{
	return sprites.GetAnimatedSpriteComponent( entity.ID );
}
*/