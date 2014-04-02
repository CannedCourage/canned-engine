//Input Class
//Handle keyboard, mouse and gamepad input (maybe more)

#ifndef _INPUT_H_
#define _INPUT_H_

#include "Logging/Log.h"

#include <vector>

#include <windows.h>

class System;
class Settings;
class Mouse;
class Keyboard;
class XboxController;
class LogicalDevice;

class Input
{
private:
protected:

	Log log;

	System& system;
	Settings& settings;

	std::vector<Mouse*> mice;
	std::vector<Keyboard*> keyboards;
	std::vector<XboxController*> pads;
	std::vector<LogicalDevice*> logicalDevices;
public:

	Input( System& s );
	~Input( void );

	void Init( void );

	void Update( void );
	void PostUpdate( void );

	void Register( Mouse* mouse );
	void Register( Keyboard* keyboard );
	void Register( XboxController* pad );
	void Register( LogicalDevice* logiDevice );

	void ReceiveRawInput( RAWINPUT* in );
};

#endif //_INPUT_H_