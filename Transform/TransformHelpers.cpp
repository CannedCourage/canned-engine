#include "Transform/TransformProcessor.h"

#include "Entity/Entity.h"

//ADD COMPONENT//

TransformAdder::TransformAdder( TransformProcessor& t ) : transforms( t )
{
}

void TransformAdder::operator()( const Entity& e )
{
	transforms.AddTransformComponent( e.ID );
}

//GET COMPONENT//

TransformGetter::TransformGetter( TransformProcessor& t ) : transforms( t )
{
}

TransformComponent& TransformGetter::operator()( const Entity& e )
{
	return transforms.GetTransformComponent( e.ID );
}

const TransformComponent& TransformGetter::operator()( const Entity& e ) const
{
	return transforms.GetTransformComponent( e.ID );
}