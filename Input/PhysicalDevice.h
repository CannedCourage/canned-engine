#ifndef _PHYSICALDEVICE_H_
#define _PHYSICALDEVICE_H_

class PhysicalDevice
{
private:
protected:
public:

	virtual void PreUpdate( void ) = 0;
	virtual void Update( void ) = 0;
	virtual void PostUpdate( void ) = 0;

	virtual void ReceiveKeyboardInput( int Key, int Scancode, int Action, int Mods ){}
	virtual void ReceiveMousePosition( double X, double Y ){}
	virtual void ReceiveMouseInput( int Button, int Action, int Mods ){}
	virtual void ReceiveScrollInput( double X, double Y ){}
};

#endif //_PHYSICALDEVICE_H_