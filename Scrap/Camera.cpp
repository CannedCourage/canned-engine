#include "Camera/Camera.h"
#include <math.h>

Camera::Camera( void ) : mode( PERSPECTIVE ), right(), up(), look(), position(), lookAt(), velocity(), 
							yaw( 0.0f ), pitch( 0.0f ), roll( 0.0f ), maxpitch( 0.0f ), maxvelocity( 0.0f ), fov( 0.0f ), aspect( 0.0f ), nearplane( 0.0f ), farplane( 0.0f )
{
}

Camera::~Camera( void )
{
}

void Camera::Forward( float units )
{
	velocity += look * units;
}

void Camera::Strafe( float units )
{
	velocity += right * units;
}

void Camera::Up( float units )
{
	velocity += up * units;
}
 
void Camera::Yaw( float radians )
{
}

void Camera::Pitch( float radians )
{
}

void Camera::Roll( float radians )
{
}

void Camera::Update()
{
	// Cap velocity to max velocity
	if ( velocity.Magnitude() > maxvelocity )
    {
		velocity.Normalize();
        velocity *= maxvelocity;
    }
 
    // Move the camera
    position += velocity;
    // Could decelerate here. I'll just stop completely.
	velocity.Clear();
    lookAt = position + look;
 
    // Calculate the new view matrix
	up.Clear();
	up.y = 1.0f;
	CreateViewMatrix( position, lookAt, up );
 
    // Set the camera axes from the view matrix
	GetVectorsFromViewMatrix();
 
    // Calculate yaw and pitch
    float lookLengthOnXZ = sqrtf( look.z * look.z + look.x * look.x );
    pitch = atan2f( look.y, lookLengthOnXZ );
    yaw   = atan2f( look.x, look.z );
}

void Camera::SetPosition( Vector* p )
{
	position.x = p->x;
    position.y = p->y;
    position.z = p->z;
}

void Camera::SetLookAt( Vector* l )
{
	lookAt.x = l->x;
    lookAt.y = l->y;
    lookAt.z = l->z;
	look = lookAt - position;
	look.Normalize();
    //D3DXVec3Normalize( &m_look, &(m_lookAt - m_position) );
}