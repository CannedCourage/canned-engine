#ifndef _TESTSCENE_H_
#define _TESTSCENE_H_

#include "SceneManager/IScene.h"
#include "Graphics/VertexBuffer.h"

#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/XboxController.h"
#include "Input/LogicalDevice.h"

#include "Transform/TransformProcessor.h"
#include "Mesh/MeshProcessor.h"
#include "Sprite/SpriteProcessor.h"
#include "Sprite/AnimatedSpriteProcessor.h"

class System;

class TestScene : public IScene
{
private:
protected:

	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;

	LPDIRECT3DVERTEXBUFFER9 cubeBuffer;
	LPDIRECT3DTEXTURE9 cubeTex;

	LPD3DXFONT font;
	RECT FontPosition;

	Keyboard keys;
	Mouse logiMouse;
	XboxController player1;
	LogicalDevice test1;

	VertexBuffer<Vertex3dTx> buffer;

	FMOD::Channel* channel;

	//Processors
	TransformProcessor transforms;
	MeshProcessor meshes;
	SpriteProcessor sprites;
	AnimatedSpriteProcessor animatedSprites;

	//Helpers
	TransformAdder 			AddTransform;
	TransformGetter 		GetTransform;
	SpriteAdder 			AddSprite;
	SpriteGetter			GetSprite;
	AnimatedSpriteAdder 	AddAnimatedSprite;
	AnimatedSpriteGetter 	GetAnimatedSprite;

	void Update( void ); //Remove

	void PreRender( void );
	void Render( void );
	void PostRender( void );
public:

	TestScene( System &s );
	~TestScene( void );
	void Load( void );
	void Unload( void );
	void OnLost( void );
	void OnRecover( void );
};

#endif //_TESTSCENE_H_