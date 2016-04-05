#ifndef _SPRITECOMPONENT_H_
#define _SPRITECOMPONENT_H_

#include <d3dx9.h>

class IDirect3DTexture9;

class RectangleFloat
{
private:
protected:
public:
	
	float top, left, right, bottom;
};

class SpriteComponent
{
private:
protected:
public:

	//This will become a transformation matrix that transforms the hard-coded texture coordinates
	RectangleFloat TextureDimensions; //Scale and Translate
	float TextureRotation = 0.0f; //Rotation about Z-axis
	
	IDirect3DTexture9* Texture = NULL; //TODO: Replace with Material component reference, Material = { texture(s), attributes, shader }?

	SpriteComponent( void );
};

typedef SpriteComponent Sprite;

#endif //_SPRITECOMPONENT_H_