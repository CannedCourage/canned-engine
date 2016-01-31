#ifndef _TRANSFORMCOMPONENT_H_
#define _TRANSFORMCOMPONENT_H_

#include "Maths/Vector.h"

struct TransformComponent
{
	Vector3D translation;
	Vector3D localRotation;
	Vector3D rotation;
	Vector3D scale;

	TransformComponent( void );
};

#endif //_TRANSFORMCOMPONENT_H_