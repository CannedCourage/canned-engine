#ifndef _XBOXCONTROLLER_H_
#define _XBOXCONTROLLER_H_

#include "Logging\Log.h"

#include <windows.h>
#include <XInput.h>

class XboxController
{
private:
protected:

	enum Button{ A, B, X, Y, LBUMPER, RBUMPER, LTHUMB, RTHUMB, START, BACK, DPADUP, DPADDOWN, DPADLEFT, DPADRIGHT };

	Log log;

	int PID;
	bool padConnected;
	XINPUT_STATE padState;
public:

	XboxController( int playerID = 0 );

	bool PadConnected( void );
	void CheckForPad( void );
	void GetPadState( void );

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