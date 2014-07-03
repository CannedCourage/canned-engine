#ifndef _SPRITECOMPONENT_H_
#define _SPRITECOMPONENT_H_

class Graphics;

#include "Maths/Vector.h"

#include <d3dx9.h>

/*struct SpriteComponent
{
	IDirect3DTexture9* texture;
	
	RECT source;
	Vector3D center;
	Vector3D translation;

	SpriteComponent( void );
	SpriteComponent( IDirect3DTexture9* _texture );
	~SpriteComponent( void );

	void SetSource( int left, int top, int right, int bottom );
};*/

struct SpriteComponent
{
	RECT texRect;
	IDirect3DTexture9* texture; //Replace with Material component reference, Material = { texture(s), attributes, shader }
};

#endif //_SPRITECOMPONENT_H_