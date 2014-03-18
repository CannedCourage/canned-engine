#ifndef _XBOXCONTROLLER_H_
#define _XBOXCONTROLLER_H_

#include "Logging\Log.h"
#include "Input\PhysicalDevice.h"

#include <windows.h>
#include <XInput.h>

class XboxController : PhysicalDevice
{
private:
protected:

	enum Button{ 	A = XINPUT_GAMEPAD_A,
					B = XINPUT_GAMEPAD_B,
					X = XINPUT_GAMEPAD_X,
					Y = XINPUT_GAMEPAD_Y,
					LBUMPER = XINPUT_GAMEPAD_LEFT_SHOULDER,
					RBUMPER = XINPUT_GAMEPAD_RIGHT_SHOULDER,
					LTHUMB = XINPUT_GAMEPAD_LEFT_THUMB,
					RTHUMB = XINPUT_GAMEPAD_RIGHT_THUMB,
					START = XINPUT_GAMEPAD_START,
					BACK = XINPUT_GAMEPAD_BACK,
					DPADUP = XINPUT_GAMEPAD_DPAD_UP,
					DPADDOWN = XINPUT_GAMEPAD_DPAD_DOWN,
					DPADLEFT = XINPUT_GAMEPAD_DPAD_LEFT,
					DPADRIGHT = XINPUT_GAMEPAD_DPAD_RIGHT };

	Log log;

	int PID;
	bool padConnected;
	XINPUT_STATE padState;
public:

	XboxController( int playerID = 0 );

	bool PadConnected( void );
	void CheckForPad( void );
	void GetPadState( void );
	void Update( void );

	bool IsPressed( Button button );
	bool WentDown( Button button );
	bool WentUp( Button button );

	//Xbox Controller

	short LeftThumbX( void );
	short LeftThumbY( void );
	short RightThumbX( void );
	short RightThumbY( void );

	BYTE LeftTrigger( void );
	BYTE RightTrigger( void );

	bool DPadUp( void );
	bool DPadDown( void );
	bool DPadLeft( void );
	bool DPadRight( void );
	bool Start( void );
	bool Back( void );
	bool LeftThumbButton( void );
	bool RightThumbButton( void );
	bool LeftBumper( void );
	bool RightBumper( void );
	bool AButton( void );
	bool BButton( void );
	bool XButton( void );
	bool YButton( void );
};

#endif //_XBOXCONTROLLER_H_