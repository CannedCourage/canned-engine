#ifndef _TRANSFORMPROCESSOR_H_
#define _TRANSFORMPROCESSOR_H_

#include "Engine/iProcess.h"

#include "Transform/TransformComponent.h"

#include "Logging/Log.h"

#include <map>

//#include "Entity/Entity.h"

class TransformProcessor : public iProcess
{
private:
protected:

	//TODO: Create typedef for map type
	//TODO: Change to unordered_map
	std::map<int, TransformComponent> transformComponents;
public:

	TransformProcessor( void );
	~TransformProcessor( void );

	void AddTransformComponent( const unsigned int entityID );
	TransformComponent& GetTransformComponent( const unsigned int entityID );
	//const TransformComponent& GetTransformComponent( const unsigned int entityID ) const;

	void Start( void );
	void Update( const double& deltaT );
	void End( void );
};

class Entity;

class TransformAdder
{
private:
protected:

	TransformProcessor& transforms;
public:

	TransformAdder( TransformProcessor& t );

	void operator()( const Entity& e );
};

class TransformGetter
{
private:
protected:

	TransformProcessor& transforms;
public:

	TransformGetter( TransformProcessor& t );

	TransformComponent& operator()( const Entity& e );
	const TransformComponent& operator()( const Entity& e ) const;
};

#endif //_TRANSFORMPROCESSOR_H_