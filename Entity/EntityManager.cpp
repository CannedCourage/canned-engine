#include "Entity/EntityManager.h"

EntityManager::EntityManager( void ) : log( "EntityManager" ), entities(), index()
{
}

EntityManager::~EntityManager( void )
{
}

Entity& EntityManager::New( const char* name )
{
	const std::string tempName( name );

	return New( tempName );
}

Entity& EntityManager::New( const std::string& name )
{
	Entity e( name );

	entities[e.ID] = e;
	//entities.insert( EntityPair( e.ID, e ) );

	//index[name] = ( e->ID );
	index.insert( IndexPair( name, e.ID ) );

	return entities[e.ID];
}

void EntityManager::Delete( const unsigned int ID )
{
	//Delete entry in entity list
	ListOfEntities::iterator it = entities.find( ID );

	if( it != entities.end() )
	{
		std::string entityName = it->second.Name;

		entities.erase( it );

		//Delete entry in index
		IndexOfEntities::iterator ix = index.find( entityName );

		if( ix != index.end() )
		{
			index.erase( ix );
		}
	}
}

void EntityManager::Delete( const char* name )
{
	std::string entityName( name );

	IndexOfEntities::iterator ix = index.find( entityName );

	if( ix != index.end() )
	{
		unsigned int ID = ix->second;

		index.erase( ix );

		ListOfEntities::iterator it = entities.find( ID );

		if( it != entities.end() )
		{
			entities.erase( it );
		}
	}
}

Entity& EntityManager::operator[]( const unsigned int ID )
{
	ListOfEntities::iterator it = entities.find( ID );

	if( it == entities.end() )
	{
		std::exception ex( "Invalid ID passed to EntityManager" );

		throw( ex );
	}

	return ( it->second );
}

Entity& EntityManager::operator[]( const char* name )
{
	std::string nameStr( name );

	IndexOfEntities::iterator it = index.find( nameStr );

	if( it == index.end() )
	{
		std::string message( "An entitiy with the name: " );
		message.append( name );
		message.append( " does not exist." );
		
		std::exception ex( message.c_str() );

		throw( ex );
	}

	return (*this)[it->second];
}

Entity& EntityManager::operator[]( const std::string& name )
{
	IndexOfEntities::iterator it = index.find( name );

	if( it == index.end() )
	{
		std::string message( "An entitiy with the name: " );
		message.append( name );
		message.append( " does not exist." );
		
		std::exception ex( message.c_str() );

		throw( ex );
	}

	return (*this)[it->second];
}

const Entity& EntityManager::operator[]( const unsigned int ID ) const
{
	const ListOfEntities::const_iterator it = entities.find( ID );

	if( it == entities.end() )
	{
		std::exception ex( "Invalid ID passed to EntityManager" );

		throw( ex );
	}

	return ( it->second );
}

const Entity& EntityManager::operator[]( const char* name ) const
{
	std::string nameStr( name );

	IndexOfEntities::const_iterator it = index.find( nameStr );

	if( it == index.end() )
	{
		std::string message( "An entitiy with the name: " );
		message.append( name );
		message.append( " does not exist." );
		
		std::exception ex( message.c_str() );

		throw( ex );
	}

	return (*this)[it->second];
}

const Entity& EntityManager::operator[]( const std::string& name ) const
{
	IndexOfEntities::const_iterator it = index.find( name );

	if( it == index.end() )
	{
		std::string message( "An entitiy with the name: " );
		message.append( name );
		message.append( " does not exist." );
		
		std::exception ex( message.c_str() );

		throw( ex );
	}

	return (*this)[it->second];
}