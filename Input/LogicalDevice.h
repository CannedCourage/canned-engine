#ifndef _LOGICALDEVICE_H_
#define _LOGICALDEVICE_H_

#include <vector>
#include <functional>

class Input;

class LogicalDevice
{
private:
protected:

	using InputFunction = std::function<bool ( void )>;
	std::vector<InputFunction> inputs;

	bool anyPressed, chordPressed;
public:

	LogicalDevice( void );
	LogicalDevice( Input& input );

	void AddInput( InputFunction func );
	void EvaluateInputs( void );

	bool IsAnyPressed( void );
	bool IsChordPressed( void );
};

#endif //_LOGICALDEVICE_H_