#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

#include "Logging/Log.h"
#include "Entity/Entity.h"

#include <vector>
#include <string>

typedef std::vector<Entity> ListOfEntities;

//TODO: Make methods that return a non-const reference to Entity private
//TODO: Add const versions of New() method?
class EntityManager
{
private:
protected:

	Log log{ "EntityManager" };

	ListOfEntities Entities;
public:

	EntityManager( void ) = default;
	~EntityManager( void ) = default;

	Entity& New( const std::string& name );
	
	void Delete( const unsigned int ID );
	void Delete( const std::string& name );

	Entity& operator[]( const unsigned int ID );
	Entity& operator[]( const std::string& name );

	const Entity& operator[]( const unsigned int ID ) const;
	const Entity& operator[]( const std::string& name ) const;
};

#endif //_ENTITYMANAGER_H_