#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <string>

class Entity
{
private:

	//This object should not be constructed from another object.
	//TODO: Create clone method? This would dupicate components as well.
	Entity( const Entity& entity );
	Entity& operator=( const Entity& entity );
protected:

	static unsigned int entityCounter;
public:

	const unsigned int ID;
	const std::string Name;

	Entity( const char* name );
	Entity( const std::string& name );
	~Entity( void );
};

#endif //_ENTITY_H_