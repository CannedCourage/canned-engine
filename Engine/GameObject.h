#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "IEngine.h"
#include "Tag.h"

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

	GameObject( void );
	~GameObject( void );
};

#endif //_GAMEOBJECT_H_