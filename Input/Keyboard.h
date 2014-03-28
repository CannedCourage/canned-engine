#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_KEYBOARD
#define HID_USAGE_GENERIC_KEYBOARD     ((USHORT) 0x06)
#endif

#include "Logging\Log.h"
#include "Input\PhysicalDevice.h"

#include <windows.h>
#include <Winuser.h>

#include <map>
#include <string>

class Keyboard : public PhysicalDevice
{
private:
protected:

	enum Keys{ 	RIGHT_CONTROL = VK_RCONTROL,
				LEFT_CONTROL = VK_LCONTROL,
				RIGHT_ALT = VK_RMENU,
				LEFT_ALT = VK_LMENU,
				NUMPAD_ENTER = VK_SEPARATOR,
				NUMPAD_0 = VK_NUMPAD0,
				NUMPAD_DECIMAL = VK_DECIMAL,
				NUMPAD_7 = VK_NUMPAD7,
				NUMPAD_1 = VK_NUMPAD1,
				NUMPAD_9 = VK_NUMPAD9,
				NUMPAD_3 = VK_NUMPAD3,
				NUMPAD_4 = VK_NUMPAD4,
				NUMPAD_6 = VK_NUMPAD6,
				NUMPAD_8 = VK_NUMPAD8,
				NUMPAD_2 = VK_NUMPAD2,
				NUMPAD_5 = VK_NUMPAD5 };

	enum KeyState{ DOWN = 1, UP = 2 };

	static Log log;

	std::map<UINT, KeyState> currentPresses, previousPresses;

	void ProcessInput( UINT& vKey, UINT& scanCode, UINT& flags );
	void HandleEscapedSequences( UINT& vKey, UINT& scanCode, UINT& flags );
public:

	Keyboard( void );

	void RegisterForRawInput( HWND hWnd );
	void ReceiveRawInput( const RAWKEYBOARD& input );

	void Update( void );
	void PostUpdate( void );

	bool IsPressed( int key );
	bool WentDown( int key );
	bool WentUp( int key );

	std::string GetKeyName( UINT virtualKey, UINT scanCode, UINT flags );
};

#endif //_KEYBOARD_H_