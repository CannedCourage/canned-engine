#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

#include "Logging/Log.h"
#include <unordered_map>
#include <string>

#include "Entity/Entity.h"

//typedef std::map<const unsigned int,Entity* const> ListOfEntities;
typedef std::unordered_map<unsigned int,Entity> ListOfEntities;
typedef std::unordered_map<std::string,const unsigned int> IndexOfEntities;

//typedef std::pair<const unsigned int,Entity* const> EntityPair;
typedef std::pair<unsigned int,Entity> EntityPair;
typedef std::pair<std::string,const unsigned int> IndexPair;

//TODO: Make methods that return a non-const reference to Entity private
//TODO: Add const versions of New() methods
//TODO: Change implementation of subscript operator to use <algorithm> functions
//i.e. (http://stackoverflow.com/questions/15517991/search-a-vector-of-objects-by-object-attribute)
/*
const Entity& operator[]( const std::string& name )
{
	auto result = std::find_if( entities.begin(), entities.end(), [&name]( const T& obj ){return obj.Name == name}; )

	if( result != entities.end() )
	{
		return result->second;
	}
}
//*/
class EntityManager
{
private:
protected:

	Log log;

	ListOfEntities entities;
	IndexOfEntities index;
public:

	EntityManager( void );
	~EntityManager( void );

	Entity& New( const char* name );
	Entity& New( const std::string& name );
	
	void Delete( const unsigned int ID );
	void Delete( const char* name );

	Entity& operator[]( const unsigned int ID );
	Entity& operator[]( const char* name );
	Entity& operator[]( const std::string& name );

	const Entity& operator[]( const unsigned int ID ) const;
	const Entity& operator[]( const char* name ) const;
	const Entity& operator[]( const std::string& name ) const;
};

#endif //_ENTITYMANAGER_H_