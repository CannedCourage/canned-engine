#include "TransformProcessor.h"

TransformProcessor::TransformProcessor( void )
{
}

TransformProcessor::~TransformProcessor( void )
{
}

void TransformProcessor::AddTransformComponent( const unsigned int entityID )
{
	TransformComponent t;

	transformComponents[entityID] = t;
}

TransformComponent& TransformProcessor::GetTransformComponent( const unsigned int entityID )
{
	return transformComponents[entityID];
}

// const TransformComponent& TransformProcessor::GetTransformComponent( const unsigned int entityID ) const
// {
// 	return transformComponents[entityID];
// }

void TransformProcessor::Start( void )
{
}

void TransformProcessor::Update( const EngineDuration& deltaT )
{
}

void TransformProcessor::End( void )
{
}