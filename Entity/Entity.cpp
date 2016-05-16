#include "Entity/Entity.h"

unsigned int Entity::EntityCounter = 0;

Entity::Entity( const std::string& Name ) : ID( EntityCounter ), Name( Name )
{
	EntityCounter++;
}

bool Entity::Has( const unsigned int ComponentTypeID ) const
{
	return ComponentFlags.test( ComponentTypeID );
}