#include "Entity/Entity.h"

unsigned int Entity::entityCounter = 0;

Entity::Entity( const char* name ) : ID( entityCounter ), Name( name )
{
	entityCounter++;
}

Entity::Entity( const std::string& name ) : ID( entityCounter ), Name( name )
{
	entityCounter++;
}

Entity::~Entity( void )
{
}