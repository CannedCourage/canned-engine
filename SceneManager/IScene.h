//Scene interface

#ifndef _ISCENE_H_
#define _ISCENE_H_

#include "Logging/Log.h"
#include "Assets/AssetManager.h"
#include "Entity/EntityManager.h"
#include "Mesh/MeshProcessor.h"

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

	//Processors
	MeshProcessor meshes;

	Log log;

	sceneStates state, nextState;	//State info
	bool loaded;					//Has the scene been loaded?
	bool lost;

	virtual void FadeIn( void ){ SetState( update ); }
	virtual void MainLoop( void ){ SetState( out ); }
	virtual void FadeOut( void ){ /*Add code to end scene, send message to scene manager?*/ Unload(); }
	virtual void RenderIn( void ){}
	virtual void RenderMain( void ){}
	virtual void RenderOut( void ){}
public:

	const char* name;

	IScene( System &sys, const char* n = "DEFAULT", sceneStates s = init );
	~IScene( void );
	virtual void Update( void );
	virtual void Render( void );
	virtual void Load( void ){}
	virtual void Unload( void ){}
	virtual const bool IsLoaded( void ) const;
	virtual void SetState( sceneStates s );
	virtual void OnLost( void ) = 0;
	virtual void OnRecover( void ) = 0;
};

#endif //_ISCENE_H_