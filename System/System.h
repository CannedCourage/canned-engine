#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "Logging/Log.h"
#include "Scene/SceneManager.h"
#include "Engine/Time.h"
#include "Assets/AssetManager.h"

#pragma warning( push )
#pragma warning( disable : 4003 ) //v2.1.1 has 100s of warnings relating to min/max macros. Fixed in v3.
#include "nlohmann/json.hpp"
#pragma warning( pop )

using json = nlohmann::json;

#include "System/Debug.h"

class WindowGLFW;
class GraphicsVK;
class Sound;
class Input;

class System
{
private:

	Log log{ "System" };

	WindowGLFW& Window;
	GraphicsVK& Graphics;
	Sound&		Sound;
	Input&		Input;

	void SetupCallbacks( void );
public:

	Time time;
	json GlobalSettings;
	AssetManager assets{ *this };
	SceneManager sceneManager{ *this };

	System( WindowGLFW& Window,	GraphicsVK& Graphics, Sound& Sound,	Input& Input );
	~System( void );

	int Init( void );
	int Run( void );
	int GameLoop( void );
	void Shutdown( void );
	void Quit( void );

	WindowGLFW* GetWindow( void ){ return &Window; }
	GraphicsVK* GetGraphics( void ){ return &Graphics; }
	Sound* GetSound( void ){ return &Sound; }
	Input* GetInput( void ){ return &Input; }
};

#endif //_SYSTEM_H_