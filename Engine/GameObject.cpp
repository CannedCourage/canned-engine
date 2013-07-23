#include "Engine\GameObject.h"
#include "System\System.h"

GameObject::GameObject( System &s ) : IEngine( s ), name( "GameObject" )
{
}

GameObject::~GameObject( void )
{
}