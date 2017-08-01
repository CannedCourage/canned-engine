#pragma once

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <Windows.h>
#include <codecvt>

#include "Logging/Log.h"
#include "Window/ConsoleAdapter.h"
#include "Scene/SceneManager.h"
#include "Window/WindowMS.h"
#include "Graphics/Graphics.h"
#include "Engine/Time.h"
#include "Input/Input.h"
#include "Assets/AssetManager.h"
#include "Sound/Sound.h"

#pragma warning( push )
#pragma warning( disable : 4003 ) //v2.1.1 has 100s of warnings relating to min/max macros. Fixed in v3.
#include "nlohmann/json.hpp"
#pragma warning( pop )

using json = nlohmann::json;

//static_assert(sizeof(float) == 4, "float size check");

#define ASSERT _ASSERTE
 
#ifdef _DEBUG
#define VERIFY ASSERT
#else
#define VERIFY(expression) (expression)
#endif

class System
{
private:
protected:

	Log log{ "System" };
	ConsoleAdapter Console;
public:

	Time time;

	//Engine
	json GlobalSettings;
	//Settings settings;
	SceneManager sceneManager{ *this };
	WindowMS window{ *this };
	Graphics graphics{ *this };
	Input input{ *this };
	Sound sound{};
	
	AssetManager assets{ *this };
	//EntityManager entityManager;?

	System( void );
	~System( void );

	int Initialise( const HINSTANCE hInstance, const LPSTR lpCmdLine, const int nCmdShow );
	int Run( void );
	int GameLoop( void );
	void Shutdown( void );
	void Quit( void );

	LRESULT CALLBACK MessageHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
};

inline void System::Quit( void )
{
	PostMessage( window.GetHandle(), WM_CLOSE, 0, 0 );
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

#endif //_SYSTEM_H_