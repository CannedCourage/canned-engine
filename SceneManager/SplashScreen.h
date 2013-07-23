//Typically the first scene loaded

#ifndef _SPLASHSCREEN_H_
#define _SPLASHSCREEN_H_

#include "SceneManager\IScene.h"

#include <d3d9.h>
#include <d3dx9.h>

class System;

class SplashScreen : public IScene
{
private:
protected:

	D3DXMATRIX matTrans;

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;

	LPDIRECT3DTEXTURE9 texture;
	float tuMin, tuMax, tvMin, tvMax;

	//TODO: Add timing code, plus skip code
	float time, timeStep, maxTime;

	void FadeIn( void );
	void MainLoop( void );
	void FadeOut( void );
	void RenderIn( void );
	void RenderMain( void );
	void RenderOut( void );
public:

	SplashScreen( System &s, const char* n = "SplashScreen" );
	~SplashScreen( void );
	void Load( void );
	void Unload( void );
	void OnLost( void );
	void OnRecover( void );
};

#endif //_SPLASHSCREEN_H_