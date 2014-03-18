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
public:

	Input( System& s );
	~Input( void );

	void Init( void );
	void Update( const float& dT );

	void Register( Mouse* mouse );
	void Register( Keyboard* keyboard );
	void Register( XboxController* pad );

	void ReceiveRawInput( RAWINPUT* in );
};

#endif //_INPUT_H_