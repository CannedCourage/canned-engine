#include "Camera\CameraDX.h"
#include "Maths\VectorDX.h"

CameraDX::CameraDX( void ) : Camera(), view( *D3DXMatrixIdentity( NULL ) ), projection( *D3DXMatrixIdentity( NULL ) )
{
}

CameraDX::~CameraDX( void )
{
}

void CameraDX::CreateViewMatrix( Vector Position, Vector LookAt, Vector Up )
{
	D3DXMatrixLookAtLH( &view, &VectorToDX( Position ), &VectorToDX( LookAt ), &VectorToDX( Up ) );
}

void CameraDX::GetVectorsFromViewMatrix( void )
{
	right.x = view._11;
    right.y = view._21;
    right.z = view._31;
    up.x = view._12;
    up.y = view._22;
    up.z = view._32;
    look.x = view._13;
    look.y = view._23;
    look.z = view._33;
}

void CameraDX::CreatePerspectiveProjection( float FOV, float Aspect, float nearPlane, float farPlane )
{
	D3DXMatrixPerspectiveFovLH( &projection, D3DXToRadian( FOV ), Aspect, nearPlane, farPlane );
}

void CameraDX::CreateOrthographicProjection( float width, float height, float nearPlane, float farPlane )
{
	D3DXMatrixOrthoLH( &projection, width, height, nearPlane, farPlane );
}

void CameraDX::Yaw( float radians )
{
	if ( radians == 0.0f )
    {
        return;
    }

    D3DXMATRIX rotation;

    D3DXMatrixRotationAxis( &rotation, &VectorToDX( up ), radians );

	right = DXToVector( *( D3DXVec3TransformNormal( &VectorToDX( right ), &VectorToDX( right ), &rotation ) ) );
	look = DXToVector( *( D3DXVec3TransformNormal( &VectorToDX( look ), &VectorToDX( look ), &rotation ) ) );
}

void CameraDX::Pitch( float radians )
{
	if ( radians == 0.0f )
    {
        return;
    }
 
	//radians = (m_invertY) ? -radians : radians;
	pitch -= radians;

    if ( pitch > maxpitch )
    {
        radians += pitch - maxpitch;
    }
    else if ( pitch < -maxpitch )
    {
        radians += pitch + maxpitch;
    }
 
    D3DXMATRIX rotation;
    D3DXMatrixRotationAxis( &rotation, &VectorToDX( right ), radians );
	
	up = DXToVector( *( D3DXVec3TransformNormal( &VectorToDX( up ), &VectorToDX( up ), &rotation ) ) );
	look = DXToVector( *( D3DXVec3TransformNormal( &VectorToDX( look ), &VectorToDX( look ), &rotation ) ) );
}

void CameraDX::Roll( float radians )
{
	if ( radians == 0.0f )
    {
        return;
    }

    D3DXMATRIX rotation;

    D3DXMatrixRotationAxis( &rotation, &VectorToDX( look ), radians );

    right = DXToVector( *( D3DXVec3TransformNormal( &VectorToDX( right ), &VectorToDX( right ), &rotation ) ) );
	up = DXToVector( *( D3DXVec3TransformNormal( &VectorToDX( up ), &VectorToDX( up ), &rotation ) ) );
}