#include "TransformProcessor.h"

TransformProcessor::TransformProcessor( void )
{
}

TransformProcessor::~TransformProcessor( void )
{
}

void TransformProcessor::AddTransformComponent( const Entity entityID )
{
	TransformComponent t;

	transformComponents[ entityID ] = t;
}

TransformComponent& TransformProcessor::GetTransformComponent( const Entity entityID )
{
	return transformComponents[ entityID ];
}

void TransformProcessor::Start( void )
{
}

void TransformProcessor::Update( const double& deltaT )
{
}

void TransformProcessor::End( void )
{
}