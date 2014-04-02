#ifndef _LOGICALDEVICE_H_
#define _LOGICALDEVICE_H_

#include <vector>

class Input;
class PhysicalDevice;

typedef bool (PhysicalDevice::*MethodPointer)(int);

class LogicalDevice
{
private:
protected:

	std::vector <PhysicalDevice*> devices;
	std::vector <MethodPointer> functions;
	std::vector <const int> args;

	bool anyPressed, chordPressed;
public:

	LogicalDevice( void );
	LogicalDevice( Input& input );

	void AddInput( PhysicalDevice* k, MethodPointer f, const int& arg );
	void EvaluateInputs( void );

	bool IsAnyPressed( void );
	bool IsChordPressed( void );
};

#endif //_LOGICALDEVICE_H_