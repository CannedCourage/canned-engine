#ifndef _PHYSICALDEVICE_H_
#define _PHYSICALDEVICE_H_

class PhysicalDevice
{
private:
protected:
public:

	virtual bool IsPressed( int input ) = 0;
	virtual bool WentDown( int input ) = 0;
	virtual bool WentUp( int input ) = 0;
};

#endif //_PHYSICALDEVICE_H_