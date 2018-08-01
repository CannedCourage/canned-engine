#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "Input/PhysicalDevice.h"

#include <map>
#include <string>

class Input;

class Keyboard : public PhysicalDevice
{
private:
protected:

	enum KeyState{ DOWN = 1, UP = 2 };

	std::map<int, KeyState> currentPresses, previousPresses;
public:
	/*
	enum Keys{ 	RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
				LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
				RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
				LEFT_ALT = 	GLFW_KEY_LEFT_ALT,
				NUMPAD_ENTER = GLFW_KEY_KP_ENTER,
				NUMPAD_0 = GLFW_KEY_KP_0,
				NUMPAD_DECIMAL = GLFW_KEY_KP_DECIMAL,
				NUMPAD_7 = GLFW_KEY_KP_7,
				NUMPAD_1 = GLFW_KEY_KP_1,
				NUMPAD_9 = GLFW_KEY_KP_9,
				NUMPAD_3 = GLFW_KEY_KP_3,
				NUMPAD_4 = GLFW_KEY_KP_4,
				NUMPAD_6 = GLFW_KEY_KP_6,
				NUMPAD_8 = GLFW_KEY_KP_8,
				NUMPAD_2 = GLFW_KEY_KP_2,
				NUMPAD_5 = GLFW_KEY_KP_5 };
	//*/

	Keyboard( void );
	Keyboard( Input& input );

	void ReceiveKeyboardInput( int Key, int Scancode, int Action, int Mods );

	void PreUpdate( void );
	void Update( void );
	void PostUpdate( void );

	bool IsPressed( int key );
	bool WentDown( int key );
	bool WentUp( int key );
};

#endif //_KEYBOARD_H_