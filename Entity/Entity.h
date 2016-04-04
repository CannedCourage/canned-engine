#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <string>
#include <bitset>

class Entity
{
private:
protected:

	static unsigned int entityCounter;

	std::bitset<64> ComponentFlags;
public:
	
	//TODO: Consider the constness of this, might be ok if only const references returned
	unsigned int ID;
	std::string Name;

	Entity( void );
	Entity( const std::string& name );
	
	Entity( const Entity& entity );

	Entity& operator=( const Entity& entity );
	
	~Entity( void );

	bool Has( unsigned int ComponentTypeID );

	//TODO: Create clone method? This would dupicate components as well.
};

#endif //_ENTITY_H_