#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "Input/PhysicalDevice.h"

#include <map>

class Input;

struct CursorPosition
{
	double X{0}, Y{0};
};

struct ScrollPosition
{
	double X{0}, Y{0};
};

class Mouse : public PhysicalDevice
{
private:
protected:

	enum ButtonState{ DOWN = 1, UP = 2 };

	std::map<int, ButtonState> CurrentPresses, PreviousPresses;

	CursorPosition CurrentPos, PreviousPos;

	ScrollPosition CurrentScroll;
public:

	/*
	enum Button{ 	
					LEFT = GLFW_MOUSE_BUTTON_LEFT,
					RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
					MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
					MOUSE4 = GLFW_MOUSE_BUTTON_4,
					MOUSE5 = GLFW_MOUSE_BUTTON_5
				};
	//*/

	Mouse( void );
	Mouse( Input& input );

	void ReceiveMousePosition( double X, double Y );
	void ReceiveMouseInput( int Button, int Action, int Mods );
	void ReceiveScrollInput( double X, double Y );

	double GetMouseXRelative( void );
	double GetMouseYRelative( void );
	double GetWheelDelta( void );

	void PreUpdate( void );
	void Update( void );
	void PostUpdate( void );

	bool IsPressed( int Button );
	bool WentDown( int Button );
	bool WentUp( int Button );
};

#endif //_MOUSE_H_