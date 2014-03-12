#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_KEYBOARD
#define HID_USAGE_GENERIC_KEYBOARD     ((USHORT) 0x06)
#endif

#include "Logging\Log.h"

#include <windows.h>

class iWindow;

class Keyboard
{
private:
protected:

	enum Key{ ENTER };

	Log log;

	RAWKEYBOARD keyboard;
public:

	Keyboard( void );

	void RegisterForRawInput( iWindow& win );
	void ReceiveRawKeyboardInput( const RAWKEYBOARD& input );

	bool IsPressed( Key key );
	bool WentDown( Key key );
	bool WentUp( Key key );
};

#endif //_KEYBOARD_H_