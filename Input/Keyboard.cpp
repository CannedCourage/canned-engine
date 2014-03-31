#include "Keyboard.h"
#include "Window\WindowMS.h"

using std::map;

Log Keyboard::log( "Keyboard" );

Keyboard::Keyboard( void )
{
}

void Keyboard::RegisterForRawInput( HWND hWnd )
{
	RAWINPUTDEVICE Rid[1];

	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    Rid[0].usUsage = HID_USAGE_GENERIC_KEYBOARD;
    Rid[0].dwFlags = NULL; //RIDEV_INPUTSINK
    Rid[0].hwndTarget = hWnd;

    RegisterRawInputDevices( Rid, 1, sizeof( Rid[0] ) );
}

void Keyboard::ReceiveRawInput( const RAWKEYBOARD& input )
{
	UINT vKey = input.VKey;
	UINT scanCode = input.MakeCode;
	UINT flags = input.Flags;

	ProcessInput( vKey, scanCode, flags );

	KeyState state;

	// a key can either produce a "make" or "break" scancode. this is used to differentiate between down-presses and releases
	// see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
	if( ( flags & RI_KEY_BREAK ) == RI_KEY_BREAK )
	{
		state = UP;
	}
	else
	{
		state = DOWN;
	}

	if( state == DOWN )
	{
		currentPresses[ vKey ] = DOWN;
	}

	if( state == UP )
	{
		currentPresses[ vKey ] = UP;
	}
}

void Keyboard::ProcessInput( UINT& virtualKey, UINT& scanCode, UINT& flags )
{
	if (virtualKey == 255)
	{
	  //Discard "fake keys" which are part of an escaped sequence
	  return;
	}
	else if (virtualKey == VK_SHIFT)
	{
	  //Virtual key does not distinguish left and right versions of SHIFT, determine based on scan code
	  virtualKey = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
	}
	else if (virtualKey == VK_NUMLOCK)
	{
	  //NUMLOCK will send same scan code as PAUSE/BREAK - need to correct,
	  //and set the extended bit (for use with GetKeyNameText)
	  scanCode = (MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC) | 0x100);
	}

	HandleEscapedSequences( virtualKey, scanCode, flags );
}

void Keyboard::HandleEscapedSequences( UINT& virtualKey, UINT& scanCode, UINT& flags )
{
	// e0 and e1 are escape sequences used for certain special keys, such as PRINT and PAUSE/BREAK.
	// see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
	const bool isE0 = ((flags & RI_KEY_E0) != 0);
	const bool isE1 = ((flags & RI_KEY_E1) != 0);
	 
	if (isE1)
	{
	  // for escaped sequences, turn the virtual key into the correct scan code using MapVirtualKey.
	  // however, MapVirtualKey is unable to map VK_PAUSE (this is a known bug), hence we map that by hand.
	  if (virtualKey == VK_PAUSE)
	    scanCode = 0x45;
	  else
	    scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);
	}

	switch (virtualKey)
	{
	  // right-hand CONTROL and ALT have their e0 bit set
	  case VK_CONTROL:
	    if (isE0)
	      virtualKey = Keys::RIGHT_CONTROL;
	    else
	      virtualKey = Keys::LEFT_CONTROL;
	    break;
	 
	  case VK_MENU:
	    if (isE0)
	      virtualKey = Keys::RIGHT_ALT;
	    else
	      virtualKey = Keys::LEFT_ALT;
	    break;
	 
	  // NUMPAD ENTER has its e0 bit set
	  case VK_RETURN:
	    if (isE0)
	      virtualKey = Keys::NUMPAD_ENTER;
	    break;
	 
	  // the standard INSERT, DELETE, HOME, END, PRIOR and NEXT keys will always have their e0 bit set, but the
	  // corresponding keys on the NUMPAD will not.
	  case VK_INSERT:
	    if (!isE0)
	    virtualKey = Keys::NUMPAD_0;
	    break;
	 
	  case VK_DELETE:
	    if (!isE0)
	      virtualKey = Keys::NUMPAD_DECIMAL;
	    break;
	 
	  case VK_HOME:
	    if (!isE0)
	      virtualKey = Keys::NUMPAD_7;
	    break;
	 
	  case VK_END:
	    if (!isE0)
	      virtualKey = Keys::NUMPAD_1;
	    break;
	 
	  case VK_PRIOR:
	    if (!isE0)
	      virtualKey = Keys::NUMPAD_9;
	    break;
	 
	  case VK_NEXT:
	    if (!isE0)
	      virtualKey = Keys::NUMPAD_3;
	    break;
	 
	  // the standard arrow keys will always have their e0 bit set, but the
	  // corresponding keys on the NUMPAD will not.
	  case VK_LEFT:
	    if (!isE0)
	      virtualKey = Keys::NUMPAD_4;
	    break;
	 
	  case VK_RIGHT:
	    if (!isE0)
	      virtualKey = Keys::NUMPAD_6;
	    break;
	 
	  case VK_UP:
	    if (!isE0)
	      virtualKey = Keys::NUMPAD_8;
	    break;
	 
	  case VK_DOWN:
	    if (!isE0)
	      virtualKey = Keys::NUMPAD_2;
	    break;
	 
	  // NUMPAD 5 doesn't have its e0 bit set
	  case VK_CLEAR:
	    if (!isE0)
	      virtualKey = Keys::NUMPAD_5;
	    break;
	}
}

void Keyboard::Update( void )
{
}

void Keyboard::PostUpdate( void )
{
	previousPresses = currentPresses;
}

bool Keyboard::IsPressed( int key )
{
	return ( currentPresses[ key ] == DOWN );
}

bool Keyboard::WentDown( int key )
{
	return ( IsPressed( key ) && ( ( previousPresses[ key ] == 0 ) || ( previousPresses[ key ] == UP ) ) );
}

bool Keyboard::WentUp( int key )
{
	return ( ( currentPresses[ key ] == UP ) && ( previousPresses[ key ] == DOWN ) );
}

std::string GetKeyName( UINT virtualKey, UINT scanCode, UINT flags )
{
	// getting a human-readable string
	const bool isE0 = ((flags & RI_KEY_E0) != 0);

	UINT key = (scanCode << 16) | (isE0 << 24);

	char buffer[512] = {};

	GetKeyNameText((LONG)key, buffer, 512);
	
	return std::string( buffer );
}