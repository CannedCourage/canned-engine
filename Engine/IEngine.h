//An interface for using engine components

#ifndef _IENGINE_H_
#define _IENGINE_H_

class System;
class SceneManager;
class Settings;
class Graphics;

class IEngine
{
private:
protected:

	//Constant interface to engine modules
	System &system;
	SceneManager &manager;
	Settings &settings;

	//Eventually remove these, move to standard set of classes for rendering (i.e. models and such)
	Graphics &graphics;

	//Input &input;
	//Sound &sound;
	//Physics* const physics;
public:

	IEngine( System &s );
	~IEngine( void );

	virtual void OnLost( void );
	virtual void OnRecover( void );
};

#endif //_IENGINE_H_