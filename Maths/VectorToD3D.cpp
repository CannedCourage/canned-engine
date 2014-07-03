#include <d3dx9.h>
#include "Maths/Vector.h"

D3DXVECTOR2 Vector2D::ToD3D( void )
{
	return D3DXVECTOR2( x, y );
}

D3DXVECTOR3 Vector3D::ToD3D( void )
{
	return D3DXVECTOR3( x, y, z );
}

D3DXVECTOR4 Vector4D::ToD3D( void )
{
	return D3DXVECTOR4( x, y, z, w );
}