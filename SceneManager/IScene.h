//Scene interface

#ifndef _ISCENE_H_
#define _ISCENE_H_

#include "Logging/Log.h"
#include "Assets/AssetManager.h"
#include "Entity/EntityManager.h"
#include "Mesh/MeshProcessor.h"

#include "Engine/ProcessManager.h"

class System;
class SceneManager;
class Settings;
class Graphics;
class Input;

enum sceneStates { init = 0 , update = 1, out = 2, };

class IScene
{
private:
protected:

	//Interface to engine modules (Services)
	System &system;
	SceneManager &manager;
	Settings &settings;

	//Services //Eventually remove these, move to standard set of classes for rendering (i.e. models and such)
	Graphics &graphics;
	Input &input;
	Sound &sound;
	//Physics* const physics;
	
	AssetManager& assets;
	EntityManager entityManager;
	ProcessManager engine;

	Log log;

	bool loaded;					//Has the scene been loaded?
	bool lost;

	virtual void MainLoop( void ){}
	virtual void RenderMain( void ){}
public:

	const char* name;

	IScene( System &sys, const char* n = "DEFAULT" );
	~IScene( void );

	virtual void PreUpdate( void );
	virtual void Update( void );
	virtual void FixedUpdate( void );
	virtual void PostUpdate( void );
	virtual void PreRender( void );
	virtual void Render( void );
	virtual void PostRender( void );

	virtual void Load( void ){}
	virtual void Unload( void ){}
	virtual const bool IsLoaded( void ) const;

	virtual void OnLost( void ) = 0;
	virtual void OnRecover( void ) = 0;
};

#endif //_ISCENE_H_