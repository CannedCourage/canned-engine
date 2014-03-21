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

class Mouse : public PhysicalDevice
{
private:
protected:

	enum Button{ 	
					LEFT = VK_LBUTTON,
					RIGHT = VK_RBUTTON,
					MIDDLE = VK_MBUTTON,
					MOUSE4 = VK_XBUTTON1,
					MOUSE5 = VK_XBUTTON2
				};

	static Log log;

	RAWMOUSE mouse;
public:

	Mouse( void );

	void RegisterForRawInput( HWND hWnd );
	void ReceiveRawInput( const RAWMOUSE& input );

	int GetMouseXRelative( void );
	int GetMouseYRelative( void );

	void Update();

	bool IsPressed( int button );
	bool WentDown( int button );
	bool WentUp( int button );
};

#endif //_MOUSE_H_