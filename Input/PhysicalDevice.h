#ifndef _PHYSICALDEVICE_H_
#define _PHYSICALDEVICE_H_

#include <windows.h>

class PhysicalDevice
{
private:
protected:
public:

	virtual void PreUpdate( void ) = 0;
	virtual void Update( void ) = 0;
	virtual void PostUpdate( void ) = 0;

	virtual void RegisterForRawInput( HWND hWnd ){}
	virtual void ReceiveRawInput( const RAWINPUT& input ){}
	virtual void ReceiveKeyboardInput( int key, int scancode, int action, int mods ){}
};

#endif //_PHYSICALDEVICE_H_