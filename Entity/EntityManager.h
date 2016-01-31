#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

#include "Logging/Log.h"
#include <vector>

#include "Entity/Entity.h"

class EntityManager
{
private:
protected:

	Log log;

	unsigned int numEntities;
public:

	EntityManager( void );

	Entity New( void );
};

#endif //_ENTITYMANAGER_H_