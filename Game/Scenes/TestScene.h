#ifndef _TESTSCENE_H_
#define _TESTSCENE_H_

#include "SceneManager/IScene.h"
#include "Graphics/VertexBuffer.h"

#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/XboxController.h"
#include "Input/LogicalDevice.h"

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
	MeshProcessor meshes;

	void MainLoop( void ); //Remove
	void RenderMain( void ); //Remove

	void PreRender( void );
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