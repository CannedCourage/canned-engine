#include "Entity/EntityManager.h"

EntityManager::EntityManager( void ) : log( "EntityManager" ), entities(), index()
{
}

EntityManager::~EntityManager( void )
{
}

Entity& EntityManager::New( const std::string& Name )
{
	auto result = index.find( Name );

	if( result != index.end() )
	{
		std::string error = "Entity with the following name already exists: " + Name;
		throw std::invalid_argument( error );
	}

	//If the implementation switches to vector, use vector::emplace() or vector::emplace_back()
	//Then the name and ID values in the Entity can become const again
	Entity e( Name );

	entities[e.ID] = e;
	//entities.insert( EntityPair( e.ID, e ) );

	//index[Name] = ( e->ID );
	index.insert( IndexPair( Name, e.ID ) );

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

void EntityManager::Delete( const std::string& Name )
{
	IndexOfEntities::iterator ix = index.find( Name );

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

//TODO: Replace with use of "at" method?
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

Entity& EntityManager::operator[]( const std::string& Name )
{
	IndexOfEntities::iterator it = index.find( Name );

	if( it == index.end() )
	{
		std::string message( "An entitiy with the name: " );
		message.append( Name );
		message.append( " does not exist." );
		
		std::exception ex( message.c_str() );

		throw( ex );
	}

	return ( *this )[it->second];
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

	return ( *this )[it->second];
}