#ifndef _MOVESCRIPT_H_
#define _MOVESCRIPT_H_

#include "Engine/Script.h"

class Entity;

class MoveScript : public Script
{
private:
protected:

	//TransformComponent& tForm;
public:

	MoveScript( Entity& GameObject );

	~MoveScript( void );

	void Start( void );

	void Update( void );
};

#endif //_MOVESCRIPT_H_