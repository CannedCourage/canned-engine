#ifndef _SPRITECOMPONENT_H_
#define _SPRITECOMPONENT_H_

#include <d3dx9.h>

struct SpriteComponent
{
	RECT texRect;
	IDirect3DTexture9* texture; //TODO: Replace with Material component reference, Material = { texture(s), attributes, shader }

	SpriteComponent( void );
};

#endif //_SPRITECOMPONENT_H_