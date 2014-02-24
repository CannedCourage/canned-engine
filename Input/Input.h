//Input Class
//Handle keyboard, mouse and gamepad input (maybe more)

#ifndef _INPUT_H_
#define _INPUT_H_

#include "Logging/Log.h"

#include <windows.h>
#include <XInput.h>

class System;
class Settings;

class Input
{
private:
protected:

	Log log;

	System& system;
	Settings& settings;

	bool padConnected[XUSER_MAX_COUNT];
	XINPUT_STATE padState[XUSER_MAX_COUNT];

	void CheckForPads( void );
	void GetPadStates( void );
public:

	Input( System& s );
	~Input( void );
};

#endif //_INPUT_H_