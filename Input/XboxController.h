#ifndef _XBOXCONTROLLER_H_
#define _XBOXCONTROLLER_H_

#pragma comment(lib, "Xinput.lib")

#include "Input\PhysicalDevice.h"

#include <windows.h>
#include <XInput.h>

class Input;

class XboxController : PhysicalDevice
{
private:
protected:

	int PID;
	bool padConnected;
	XINPUT_STATE currentState, previousState;
	float deadLX, deadLY, deadRX, deadRY;
	float adjLX, adjLY, adjRX, adjRY;
	float trigL, trigR;

	float NormaliseStick( int stickOutput );
	float AdjustNormOutput( float normalisedOutput, float deadzone );
	float NormaliseTrigger( int trigOutput );
public:

	enum Button
	{
		A = XINPUT_GAMEPAD_A,
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
		DPADRIGHT = XINPUT_GAMEPAD_DPAD_RIGHT
	};

	XboxController( int playerID = 0, float _deadLX = 0.0f, float _deadLY = 0.0f, float _deadRX = 0.0f, float _deadRY = 0.0f );
	XboxController( Input& input, int playerID = 0, float _deadLX = 0.0f, float _deadLY = 0.0f, float _deadRX = 0.0f, float _deadRY = 0.0f );

	bool PadConnected( void );
	void GetPadState( void );

	void PreUpdate( void );
	void Update( void );
	void PostUpdate( void );

	void Vibrate( int leftVal = 0, int rightVal = 0 );

	void SetDeadzone( float _deadLX = 0.0f, float _deadLY = 0.0f, float _deadRX = 0.0f, float _deadRY = 0.0f );

	//Digital

	bool IsPressed( int button );
	bool WentDown( int button );
	bool WentUp( int button );

	//Analogue

	float LeftThumbX( void );
	float LeftThumbY( void );
	float RightThumbX( void );
	float RightThumbY( void );

	float LeftTrigger( void );
	float RightTrigger( void );
};

#endif //_XBOXCONTROLLER_H_