#ifndef _TRANSFORMCOMPONENT_H_
#define _TRANSFORMCOMPONENT_H_

#include "Maths/Vector.h"

class TransformComponent
{
private:
protected:
public:
	
	Vector3D translation;
	Vector3D localRotation;
	Vector3D rotation;
	Vector3D scale;

	TransformComponent( void );
};

typedef TransformComponent Transform;

#endif //_TRANSFORMCOMPONENT_H_