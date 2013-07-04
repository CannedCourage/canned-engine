//Scene interface

#ifndef _ISCENE_H_
#define _ISCENE_H_

#include "Engine\IEngine.h"
#include "Logging\ILoggable.h"
#include "Camera\Camera.h"

enum sceneStates { init = 0 , update = 1, out = 2, };

class IScene : public IEngine, public ILoggable
{
private:
protected:

	sceneStates state, nextState;	//State info
	bool loaded;					//Has the scene been loaded?
	bool lost;

	Camera* mainCamera;

	virtual void FadeIn( void ) = 0;
	virtual void MainLoop( void ) = 0;
	virtual void FadeOut( void ) = 0;
	virtual void RenderIn( void ) = 0;
	virtual void RenderMain( void ) = 0;
	virtual void RenderOut( void ) = 0;
public:

	const char* name;

	IScene( const char* n = "DEFAULT", sceneStates s = init );
	~IScene( void );
	virtual void Update( void );
	virtual void Render( void );
	virtual void Load( void ) = 0;
	virtual void Unload( void ) = 0;
	virtual const bool IsLoaded( void ) const;
	virtual void SetState( sceneStates s );
};

#endif //_ISCENE_H_