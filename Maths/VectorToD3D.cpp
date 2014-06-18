#include "Maths/VectorToD3D.h"

D3DXVECTOR3 VectorToD3D( const Vector3D& vec )
{
	return D3DXVECTOR3( vec.x, vec.y, vec.z );
}