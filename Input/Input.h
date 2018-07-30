//Input Class
//Handle keyboard, mouse and gamepad input (maybe more)

#ifndef _INPUT_H_
#define _INPUT_H_

#include "Logging/Log.h"
#include "Engine/Event.h"

#include <vector>
#include <windows.h>

class PhysicalDevice;
class LogicalDevice;

class Input
{
private:
protected:

	Log log;

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

	void ReceiveRawInput( RAWINPUT* in );
};

#endif //_INPUT_H_