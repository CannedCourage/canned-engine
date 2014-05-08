#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

#include "Logging/Log.h"
#include <vector>

typedef unsigned int Entity;

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