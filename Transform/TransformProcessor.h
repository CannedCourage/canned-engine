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

	std::map<int, TransformComponent> transformComponents;
public:

	TransformProcessor( void );
	~TransformProcessor( void );

	void AddTransformComponent( const unsigned int entityID );
	TransformComponent& GetTransformComponent( const unsigned int entityID );

	void Start( void );
	void Update( const double& deltaT );
	void End( void );
};

#endif //_TRANSFORMPROCESSOR_H_