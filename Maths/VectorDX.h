#ifndef _VECTORDX_H_
#define _VECTORDX_H_

#include "Maths\Vector.h"
#include <d3d9.h>
#include <d3dx9.h>

D3DXVECTOR3 VectorToDX( const Vector& V );
Vector DXToVector( const D3DXVECTOR3& V );

#endif //_VECTORDX_H_