#include "Entity/Entity.h"
#include "Engine/Script.h"

#include <algorithm>

unsigned int Entity::EntityCounter = 0;

Entity::Entity( const std::string& Name ) : ID( EntityCounter ), Name( Name )
{
	EntityCounter++;
}

bool Entity::Has( const unsigned int ComponentTypeID ) const
{
	return ComponentFlags.test( ComponentTypeID );
}

void Entity::AddScript( Script* ScriptInstance )
{
	//TODO: Check that only one instance of a script per type is allowed
	Scripts.push_back( ScriptInstance );
}

void Entity::DeleteScript( std::string& Name )
{
	//const unique_ptr<SomeLargeData>& a
	auto result = std::find_if( Scripts.begin(), Scripts.end(), [ &Name ]( const Script* ScriptInstance )
	{
		if( ScriptInstance->Name == Name )
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if( result != Scripts.end() )
	{
		Scripts.erase( result );
	}
}