#include "Engine\IEngine.h"
#include "SceneManager\SceneManager.h"

IEngine::IEngine( void ) : manager( SceneManager::Get() ), settings( &(manager->settings) ),// window( &(manager->window) ),
	graphics( &( manager->graphics ) ), Interface( graphics->Interface() ), device( graphics->Device() ),
	input( &( manager->input ) ),
	sound( &( manager->sound ) )
{
}

IEngine::~IEngine( void )
{
}

void IEngine::OnLost( void )
{
	//Device has been lost, destroy resources
}

void IEngine::OnRecover( void )
{
	//Device has recovered, recreate resources
}