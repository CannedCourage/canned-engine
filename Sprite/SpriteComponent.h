#ifndef _SPRITECOMPONENT_H_
#define _SPRITECOMPONENT_H_

#include <d3dx9.h>

struct SpriteComponent
{
	//Store textureCoordinates as coordinates, but will become a transformation matrix
	RECT texRect;

	//Apply rotation about Z-axis in the followng amount
	float rotation;
	
	IDirect3DTexture9* texture; //TODO: Replace with Material component reference, Material = { texture(s), attributes, shader }?

	SpriteComponent( void );
};

#endif //_SPRITECOMPONENT_H_