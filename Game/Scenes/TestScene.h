#ifndef _TESTSCENE_H_
#define _TESTSCENE_H_

#include "SceneManager\IScene.h"
#include "Graphics\VertexBuffer.h"

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

	VertexBuffer<Vertex3dTx> buffer;

	void FadeIn( void );
	void MainLoop( void );
	void FadeOut( void );
	void RenderIn( void );
	void RenderMain( void );
	void RenderOut( void );
public:

	TestScene( System &s );
	~TestScene( void );
	void Load( void );
	void Unload( void );
	void OnLost( void );
	void OnRecover( void );
};

#endif //_TESTSCENE_H_