#include "Entity/Entity.h"

unsigned int Entity::entityCounter = 0;

Entity::Entity( void )
{
}

Entity::Entity( const std::string& name ) : ID( entityCounter ), Name( name )
{
	entityCounter++;
}

Entity::Entity( const Entity& entity ) : ID( entity.ID ), Name( entity.Name )
{
}

Entity& Entity::operator=( const Entity& entity )
{
	this->ID = entity.ID;
	this->Name = entity.Name;

	return ( *this );
}

Entity::~Entity( void )
{
}

bool Entity::Has( unsigned int ComponentTypeID )
{
	return ComponentFlags.test( ComponentTypeID );
}