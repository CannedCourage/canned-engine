#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Engine\IEngine.h"
#include "Engine\Tag.h"

class System;

class GameObject : public IEngine
{
private:
protected:

	//Transform, Collider, Rigidbody, etc.
	//Log?
	char* name;
	Tag* tag;

	//TODO: Put engine interface in components instead?
public:

	GameObject( System &s );
	~GameObject( void );
};

#endif //_GAMEOBJECT_H_