#include "Maths\VectorDX.h"

D3DXVECTOR3 VectorToDX( const Vector& V )
{
	return D3DXVECTOR3( V.x, V.y, V.z );
}

Vector DXToVector( const D3DXVECTOR3& V )
{
	return Vector( V.x, V.y, V.z );
}