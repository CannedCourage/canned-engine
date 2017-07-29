#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#include <string>

class Entity;

class Script
{
private:
protected:

	Entity& GameObject;
public:
	
	const std::string Name;
	bool Active = true;

	Script(const Script&) = delete;
    Script& operator=(const Script&) = delete;
    
	Script( Entity& GameObject, const std::string& Name ) : GameObject( GameObject ), Name( Name )
	{
	}

	virtual ~Script( void )
	{
	}

	virtual void Start( void ){}

	//Game Loop Events
	virtual void PreUpdate( void ){}
	virtual void Update( void ){}
	virtual void PostUpdate( void ){}

	virtual void PreRender( void ){}
	virtual void Render( void ){}
	virtual void PostRender( void ){}
};

#endif //_SCRIPT_H_