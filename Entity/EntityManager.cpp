#include "Entity/EntityManager.h"

EntityManager::EntityManager( void ) : log( "EntityManager" ), numEntities( 0 )
{
}

Entity EntityManager::New( void )
{
	return numEntities++;
}