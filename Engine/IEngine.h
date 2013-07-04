//An interface for using engine components

#ifndef _IENGINE_H_
#define _IENGINE_H_

#include "Engine\Modules.h"

class SceneManager;

class IEngine
{
private:
protected:

	//Constant interface to engine modules
	SceneManager* const manager;
	DefaultSettings* const settings; //Make const to prevent scenes etc. being able to change settings?

	Graphics* const graphics;
	LPDIRECT3D9 const Interface;
	LPDIRECT3DDEVICE9 const device;

	Input* const input;
	Sound* const sound;
	//Physics* const physics;
public:

	IEngine( void );
	~IEngine( void );

	virtual void OnLost( void );
	virtual void OnRecover( void );
};

#endif //_IENGINE_H_