#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <string>
#include <bitset>

class Entity
{
private:
protected:

	static unsigned int EntityCounter;

	std::bitset<64> ComponentFlags;
public:
	
	//TODO: Consider the constness of this, might be ok if only const references returned
	unsigned int ID;
	std::string Name;

	Entity( void ) = delete;
	Entity( const std::string& Name );
	Entity( const Entity& Entity ) = default;
	~Entity( void ) = default;

	Entity& operator=( const Entity& rhs ) = default;

	bool Has( const unsigned int ComponentTypeID ) const;

	//TODO: Create clone method? This would dupicate components as well.
};

#endif //_ENTITY_H_