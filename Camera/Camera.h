#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Maths\Vector.h"

enum cameraMode{ PERSPECTIVE, ORTHOGRAPHIC };

class Camera
{
private:
protected:

	cameraMode	mode;

	Vector		right;
	Vector		up;
	Vector		look;
	Vector		position;
	Vector		lookAt;
	Vector		velocity;

	float		yaw;
	float		pitch;
	float		roll;
	float		maxpitch;
	float		maxvelocity;
	float		fov;
	float		aspect;
	float		nearplane;
	float		farplane;

	virtual void CreateViewMatrix( Vector Position, Vector LookAt, Vector Up ) = 0;
	virtual void GetVectorsFromViewMatrix( void ) = 0;
public:

	Camera( void );
	~Camera( void );

	virtual void CreatePerspectiveProjection( float FOV, float Aspect, float nearPlane, float farPlane ) = 0;
	virtual void CreateOrthographicProjection( float width, float height, float nearPlane, float farPlane ) = 0;

    void Forward( float units );
    void Strafe( float units );
    void Up( float units );
 
	virtual void Yaw( float radians );
	virtual void Pitch( float radians );
	virtual void Roll( float radians );

    void Update();

	//mutators
    void SetPosition( Vector* p );
    void SetLookAt( Vector* l );

    void SetFOV( float f ){ CreatePerspectiveProjection( f, aspect, nearplane, farplane ); }
    void SetAspectRatio( float a ){ CreatePerspectiveProjection( fov, a, nearplane, farplane ); }
    void SetNearPlane( float p ){ CreatePerspectiveProjection( fov, aspect, p, farplane ); }
    void SetFarPlane( float p ){ CreatePerspectiveProjection( fov, aspect, nearplane, p ); }
    void SetMaxVelocity( float maxVelocity ) { maxvelocity = maxVelocity; }

	//accessors
    Vector* GetPosition()				{ return &position; }
    Vector* GetLookAt()					{ return &lookAt; }

    float GetFOV()						{ return fov; }
    float GetAspectRatio()				{ return aspect; }
    float GetNearPlane()				{ return nearplane; }
    float GetFarPlane()					{ return farplane; }
    float GetMaxVelocity()				{ return maxvelocity; }
    float GetPitch()					{ return pitch; }
    float GetYaw()						{ return yaw; }
    float GetMaxPitch()					{ return maxpitch; }
	Vector GetVelocity(void)			{ return velocity; }
};

#endif //_CAMERA_H_