//Input Class
//Handle keyboard, mouse and gamepad input (maybe more)

#ifndef _INPUT_H_
#define _INPUT_H_

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

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

	RAWINPUTDEVICE Rid[1];
	RAWMOUSE mouse;

	//Xbox Controller

	void CheckForPads( void );
	void GetPadStates( void );

	//Mouse

	void RegisterMouseForRawInput( void );

	//Keyboard
public:

	Input( System& s );
	~Input( void );

	void Init( void );
	void Update( void );

	//Xbox Controller

	bool PadConnected( unsigned short player );

	short LeftThumbX( unsigned short player );
	short LeftThumbY( unsigned short player );
	short RightThumbX( unsigned short player );
	short RightThumbY( unsigned short player );

	BYTE LeftTrigger( unsigned short player );
	BYTE RightTrigger( unsigned short player );

	bool DPadUp( unsigned short player );
	bool DPadDown( unsigned short player );
	bool DPadLeft( unsigned short player );
	bool DPadRight( unsigned short player );
	bool Start( unsigned short player );
	bool Back( unsigned short player );
	bool LeftThumbButton( unsigned short player );
	bool RightThumbButton( unsigned short player );
	bool LeftBumper( unsigned short player );
	bool RightBumper( unsigned short player );
	bool A( unsigned short player );
	bool B( unsigned short player );
	bool X( unsigned short player );
	bool Y( unsigned short player );

	//Mouse
	
	void ReceiveRawMouseInput( const RAWMOUSE& input );
	int GetMouseXRelative( void );
	int GetMouseYRelative( void );

	//Keyboard
};

#endif //_INPUT_H_