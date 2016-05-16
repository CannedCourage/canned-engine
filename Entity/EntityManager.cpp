#include "Entity/EntityManager.h"

#include <algorithm>

Entity& EntityManager::New( const std::string& Name )
{
	//TODO: Switch to set, enforces key uniqueness
	auto result = std::find_if( Entities.begin(), Entities.end(), [&Name]( const Entity& Obj ){ return ( Obj.Name == Name ); } );

	if( result != Entities.end() )
	{
		std::string error = "Entity with the following name already exists: " + Name;
		throw std::invalid_argument( error );
	}

	auto iteratorToNewEntity = Entities.emplace( Entities.end(), Entity( Name ) );

	return ( *iteratorToNewEntity );
}

//Delete entry in entity list based on ID
void EntityManager::Delete( const unsigned int ID )
{
	std::remove_if( Entities.begin(), Entities.end(), [&ID]( const Entity& Obj ){ return ( Obj.ID == ID ); } );
}

//Delete entry in entity list based on Name
void EntityManager::Delete( const std::string& Name )
{
	std::remove_if( Entities.begin(), Entities.end(), [&Name]( const Entity& Obj ){ return ( Obj.Name == Name ); } );
}

//Get reference to Entity based on ID, throw if Entity does not exist
Entity& EntityManager::operator[]( const unsigned int ID )
{
	auto result = std::find_if( Entities.begin(), Entities.end(), [&ID]( const Entity& Obj ){ return ( Obj.ID == ID ); } );

	if( result != Entities.end() )
	{
		return ( *result );
	}
	else
	{
		std::string error = "Entity with the following ID does not exist: " + std::to_string( ID );
		throw std::invalid_argument( error );
	}
}

//Get reference to Entity based on Name, throw if Entity does not exist
Entity& EntityManager::operator[]( const std::string& Name )
{
	auto result = std::find_if( Entities.begin(), Entities.end(), [&Name]( const Entity& Obj ){ return ( Obj.Name == Name ); } );

	if( result != Entities.end() )
	{
		return ( *result );
	}
	else
	{
		std::string error = "Entity with the following Name does not exist: " + Name;
		throw std::invalid_argument( error );
	}
}

const Entity& EntityManager::operator[]( const unsigned int ID ) const
{
	const auto result = std::find_if( Entities.begin(), Entities.end(), [&ID]( const Entity& Obj ){ return ( Obj.ID == ID ); } );

	if( result != Entities.end() )
	{
		return ( *result );
	}
	else
	{
		std::string error = "Entity with the following ID does not exist: " + std::to_string( ID );
		throw std::invalid_argument( error );
	}
}

const Entity& EntityManager::operator[]( const std::string& Name ) const
{
	const auto result = std::find_if( Entities.begin(), Entities.end(), [&Name]( const Entity& Obj ){ return ( Obj.Name == Name ); } );

	if( result != Entities.end() )
	{
		return ( *result );
	}
	else
	{
		std::string error = "Entity with the following Name does not exist: " + Name;
		throw std::invalid_argument( error );
	}
}