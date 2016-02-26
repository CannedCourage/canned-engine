#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

#include "Logging/Log.h"
#include <map>
#include <string>

#include "Entity/Entity.h"

typedef std::map<const unsigned int,Entity* const> ListOfEntities;
typedef std::map<const std::string,const unsigned int> IndexOfEntities;

typedef std::pair<const unsigned int,Entity* const> EntityPair;
typedef std::pair<const std::string,const unsigned int> IndexPair;

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