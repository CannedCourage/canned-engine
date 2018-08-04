#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <string>
#include <bitset>
#include <memory>
#include <vector>

class Script;

class Entity
{
private:
protected:

	static unsigned int EntityCounter;

	std::bitset<64> ComponentFlags;

	std::vector<Script*> Scripts;
public:
	
	//TODO: Consider the constness of this, might be ok if only const references returned
	//These being const causes compilation error with certain <algorithm> functions?
	unsigned int ID;
	std::string Name;

	Entity( void ) = delete;
	Entity( const std::string& Name );
	Entity( const Entity& Entity ) = default;
	~Entity( void ) = default;

	Entity& operator=( const Entity& rhs ) = default;

	operator int() const;
	operator unsigned int() const;

	bool Has( const unsigned int ComponentTypeID ) const;

	void AddScript( Script* ScriptInstance );
	void DeleteScript( std::string& Name );

	//TODO: Create clone method? This would dupicate components as well.
};

#endif //_ENTITY_H_