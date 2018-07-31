#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <Windows.h>
#include <codecvt>
#include <memory>

#include "Logging/Log.h"
#include "Window/ConsoleAdapter.h"
#include "Scene/SceneManager.h"
#include "Engine/Time.h"
#include "Input/Input.h"
#include "Assets/AssetManager.h"
#include "Sound/Sound.h"

#pragma warning( push )
#pragma warning( disable : 4003 ) //v2.1.1 has 100s of warnings relating to min/max macros. Fixed in v3.
#include "nlohmann/json.hpp"
#pragma warning( pop )

using json = nlohmann::json;

#include "System/Debug.h"

class WindowGLFW;
class GraphicsVK;

class System
{
private:
protected:

	Log log{ "System" };
	ConsoleAdapter Console;

	std::unique_ptr<WindowGLFW> Window;
	std::unique_ptr<GraphicsVK> Graphics;
public:

	Time time;

	//Engine
	json GlobalSettings;
	Sound sound;
	Input input;
	
	AssetManager assets{ *this };

	SceneManager sceneManager{ *this };

	System( void );
	~System( void );

	int Initialise( void );
	int Run( void );
	int GameLoop( void );
	void Shutdown( void );
	void Quit( void );

	WindowGLFW* GetWindow( void ){ return Window.get(); }
	GraphicsVK* GetGraphics( void ){ return Graphics.get(); }
};

#endif //_SYSTEM_H_