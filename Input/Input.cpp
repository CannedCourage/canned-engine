#include "Input/Input.h"
#include "Input/PhysicalDevice.h"
#include "Input/LogicalDevice.h"

Input::Input( void )
{
}

Input::~Input( void )
{
}

void Input::Init( void )
{
}

void Input::PreUpdate( void )
{
	for( PhysicalDevice* device : physicalDevices )
	{
		device->PreUpdate();
	}
}

void Input::Update( void )
{
	for( PhysicalDevice* device : physicalDevices )
	{
		device->Update();
	}
	
	for( LogicalDevice* device : logicalDevices )
	{
		device->EvaluateInputs();
	}
}

void Input::PostUpdate( void )
{
	for( PhysicalDevice* device : physicalDevices )
	{
		device->PostUpdate();
	}
}

void Input::Register( PhysicalDevice* physDevice )
{
	physicalDevices.push_back( physDevice );
}

void Input::Register( LogicalDevice* intent )
{
	logicalDevices.push_back( intent );
}

void Input::ReceiveKeyboardInput( int Key, int Scancode, int Action, int Mods )
{
	for( PhysicalDevice* device : physicalDevices )
	{
		device->ReceiveKeyboardInput( Key, Scancode, Action, Mods );
	}
}

void Input::ReceiveMousePosition( double X, double Y )
{
	for( PhysicalDevice* device : physicalDevices )
	{
		device->ReceiveMousePosition( X, Y );
	}
}

void Input::ReceiveMouseInput( int Button, int Action, int Mods )
{
	for( PhysicalDevice* device : physicalDevices )
	{
		device->ReceiveMouseInput( Button, Action, Mods );
	}
}

void Input::ReceiveScrollInput( double X, double Y )
{
	for( PhysicalDevice* device : physicalDevices )
	{
		device->ReceiveScrollInput( X, Y );
	}
}