//Input Class
//Handle keyboard, mouse and gamepad input (maybe more)

#ifndef _INPUT_H_
#define _INPUT_H_

//#include "Engine/Event.h"

#include <vector>

class PhysicalDevice;
class LogicalDevice;

class Input
{
private:
protected:

	std::vector<PhysicalDevice*> physicalDevices;
	std::vector<LogicalDevice*> logicalDevices;

	// Event<void ( const RAWKEYBOARD& )> KeyboardRawInput;
	// Event<void ( void )> UpdatePhysicalDevices;
	// Event<void ( void )> EvaluateLogicalDevices;
public:

	Input( void );
	~Input( void );

	void Init( void );

	void PreUpdate( void );
	void Update( void );
	void PostUpdate( void );

	void Register( PhysicalDevice* physDevice );
	void Register( LogicalDevice* logiDevice );
	
	void ReceiveKeyboardInput( int Key, int Scancode, int Action, int Mods );
	void ReceiveMousePosition( double X, double Y );
	void ReceiveMouseInput( int Button, int Action, int Mods );
	void ReceiveScrollInput( double X, double Y );
};

#endif //_INPUT_H_