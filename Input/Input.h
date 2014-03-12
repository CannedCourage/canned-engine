//Input Class
//Handle keyboard, mouse and gamepad input (maybe more)

#ifndef _INPUT_H_
#define _INPUT_H_

#include "Logging/Log.h"

#include "Input\XboxController.h"
#include "Input\Mouse.h"
#include "Input\Keyboard.h"

class System;
class Settings;

class Input
{
private:
protected:

	Log log;

	System& system;
	Settings& settings;

	XboxController pad1, pad2, pad3, pad4;
	Mouse mouse;
	Keyboard keyboard;
public:

	Input( System& s );
	~Input( void );

	XboxController& Controller( int number = 0 );
	Mouse& Mouse( void );
	Keyboard& Keyboard( void );

	void Init( void );
	void Update( void );
};

#endif //_INPUT_H_