#include "Game/Scripts/MoveScript.h"
#include "Entity/Entity.h"

MoveScript::MoveScript( Entity& GameObject ) : Script( GameObject, "MoveScript" )
{
}

MoveScript::~MoveScript( void )
{
}

void MoveScript::Start( void )
{
	//tForm = GetTransform( GameObject );
}

void MoveScript::Update( void )
{
	//tForm.translation.x++;
}