#ifndef _MOUSE_H_
#define _MOUSE_H_

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

#include "Logging\Log.h"
#include "Input\PhysicalDevice.h"

#include <windows.h>
#include <map>

class Input;

class Mouse : public PhysicalDevice
{
private:
protected:

	std::map<int, bool> buttonStates;

	static Log log;

	RAWMOUSE currentMouse;
public:

	enum Button{ 	
					LEFT = RI_MOUSE_BUTTON_1_DOWN,
					RIGHT = RI_MOUSE_BUTTON_2_DOWN,
					MIDDLE = RI_MOUSE_BUTTON_3_DOWN,
					MOUSE4 = RI_MOUSE_BUTTON_4_DOWN,
					MOUSE5 = RI_MOUSE_BUTTON_5_DOWN
				};

	Mouse( void );
	Mouse( Input& input );

	void RegisterForRawInput( HWND hWnd );
	void ReceiveRawInput( const RAWINPUT& input );

	long GetMouseXRelative( void );
	long GetMouseYRelative( void );

	short GetWheelDelta( void );

	void PreUpdate( void );
	void Update( void );
	void PostUpdate( void );

	bool IsPressed( int button );
	bool WentDown( int button );
	bool WentUp( int button );
};

#endif //_MOUSE_H_