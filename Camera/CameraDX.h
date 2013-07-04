#ifndef _CAMERADX_H_
#define _CAMERADX_H_

#include "Camera\Camera.h"

#include <d3d9.h>
#include <d3dx9.h>

class CameraDX : public Camera
{
private:
protected:

	D3DXMATRIX view;
	D3DXMATRIX projection;

	void CreateViewMatrix( Vector Position, Vector LookAt, Vector Up );
	void GetVectorsFromViewMatrix( void );
public:

	CameraDX( void );
	~CameraDX( void );

	void CreatePerspectiveProjection( float FOV, float Aspect, float nearPlane, float farPlane );
	void CreateOrthographicProjection( float width, float height, float nearPlane, float farPlane );

	void Yaw( float radians );
    void Pitch( float radians );
    void Roll( float radians );

	D3DXMATRIX* GetViewMatrix()        { return &view; }
    D3DXMATRIX* GetProjectionMatrix()  { return &projection; }
};

#endif //_CAMERADX_H_