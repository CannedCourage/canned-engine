#include "Input/Input.h"
#include "System/System.h"
#include "Input/PhysicalDevice.h"
#include "Input/LogicalDevice.h"

#include <algorithm>

Input::Input( System& s ) : log( "Input" ), system( s ) , settings( system.settings )
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
	std::for_each( physicalDevices.begin(), physicalDevices.end(), []( PhysicalDevice* device )
	{
		device->PreUpdate();
	});
}

void Input::Update( void )
{
	std::for_each( physicalDevices.begin(), physicalDevices.end(), []( PhysicalDevice* device )
	{
		device->Update();
	});
	
	std::for_each( logicalDevices.begin(), logicalDevices.end(), []( LogicalDevice* device )
	{
		device->EvaluateInputs();
	});
}

void Input::PostUpdate( void )
{
	std::for_each( physicalDevices.begin(), physicalDevices.end(), []( PhysicalDevice* device )
	{
		device->PostUpdate();
	});
}

void Input::Register( PhysicalDevice* physDevice )
{
	physicalDevices.push_back( physDevice );

	physDevice->RegisterForRawInput( system.window.getHandle() );
}

void Input::Register( LogicalDevice* intent )
{
	logicalDevices.push_back( intent );
}

void Input::ReceiveRawInput( RAWINPUT* in )
{
	std::for_each( physicalDevices.begin(), physicalDevices.end(), [in]( PhysicalDevice* device )
	{
		device->ReceiveRawInput( *in );
	});
}