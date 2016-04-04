//Scene interface

#ifndef _ISCENE_H_
#define _ISCENE_H_

#include "Logging/Log.h"
#include "Entity/EntityManager.h"
#include "Engine/ProcessManager.h"

#include <string>

class System;
class SceneManager;
class Settings;
class Graphics;
class Input;
class Sound;
class AssetManager;

enum class SceneState : unsigned int
{
	INIT,
	UPDATE,
	CLEANUP,
	END
};

class IScene
{
private:
protected:

	//Interface to engine modules (Services)
	System& system;
	SceneManager& manager;
	Settings& settings;

	//Services //Eventually remove these, move to standard set of classes for rendering (i.e. models and such)
	Graphics& graphics;
	Input& input;
	Sound& sound;
	//Physics& physics;
	
	AssetManager& assets;
	EntityManager entityManager;
	ProcessManager engine;

	Log log;

	bool loaded = false;					//Has the scene been loaded?
	bool lost = false;
public:

	const std::string Name;

	SceneState State = SceneState::INIT;

	IScene( System &sys, const std::string& N = "DEFAULT" );
	virtual ~IScene( void );

	virtual void Load( void ) = 0;

	virtual void Start( void ) = 0;
	
	virtual void PreUpdate( void );
	virtual void Update( void );
	virtual void FixedUpdate( void );
	virtual void PostUpdate( void );
	
	virtual void PreRender( void );
	virtual void Render( void );
	virtual void PostRender( void );
	
	virtual void End( void ) = 0;
	
	virtual void Unload( void ) = 0;

	virtual const bool IsLoaded( void ) const;

	virtual void OnLost( void ) = 0;
	virtual void OnRecover( void ) = 0;
};

#endif //_ISCENE_H_