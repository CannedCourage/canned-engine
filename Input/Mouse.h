#ifndef _MOUSE_H_
#define _MOUSE_H_

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

#include "Logging\Log.h"

#include <windows.h>

class iWindow;

class Mouse
{
private:
protected:

	enum Button{ LEFT, RIGHT, MIDDLE, MOUSE4, MOUSE5 };

	Log log;

	RAWMOUSE mouse;
public:

	Mouse( void );

	void RegisterForRawInput( iWindow& win );
	void ReceiveRawMouseInput( const RAWMOUSE& input );
	int GetMouseXRelative( void );
	int GetMouseYRelative( void );

	bool IsPressed( Button button );
	bool WentDown( Button button );
	bool WentUp( Button button );
};

#endif //_MOUSE_H_