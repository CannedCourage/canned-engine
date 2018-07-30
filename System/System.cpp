#define settingsFile "w:/engine/data/settings/MainSettings.json"

#include "System/System.h"
#include "StandardResources/resource.h"

#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3dll.lib")

System::System( void )
{
	std::ifstream mainSettingsFile{ settingsFile };
	
	mainSettingsFile >> GlobalSettings;

	//Console.CreateConsole( true, false );
}

System::~System( void )
{
	std::ofstream mainSettingsFile{ settingsFile };

	mainSettingsFile << std::setw(4) << GlobalSettings << std::endl;
}

int System::Initialise( void )
{
	TRACE( "System Init" );

	glfwInit();

    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API ); //No OpenGL
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE ); //Prevent resizing

    window = glfwCreateWindow( 800, 600, "GLFW window", nullptr, nullptr );
    glfwSetWindowUserPointer( window, this ); //Make System available to GLFW

    if( !window )
    {
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);
	
	newGraphics.Window = window;
	newGraphics.Init();

	sound.Init();

	glfwSetKeyCallback( window, key_callback );

	input.Init();

	//assets?
	sceneManager.Init();

	return 1;
}

int System::Run( void )
{
	//Main loop
	//The Message Loop
	//MSG msg; //A system message
	
	while( !glfwWindowShouldClose(window) )
    {
    	time.FrameBegin();

    	/* Poll for and process events */
        glfwPollEvents();

    	if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
		{
			time.FrameEnd();
			Quit();
			break;
		}

		GameLoop();

		time.FrameEnd();
    }
    //*/

    return 0;
	//return msg.wParam;	//If message is quit, this will be 0. If there was an error, this will be -1
	//~The Message Loop
}

int System::GameLoop( void )
{
	time.AddToPhysicsAccumulator( time.DeltaTimeActual() );

	sceneManager.Prepare();

	input.Update();
	sceneManager.Update();

	while( time.GetPhysicsAccumulator() > time.FixedStepActual() )
	{
		if( time.FixedStepActual() == EngineDuration::zero() ){ break; }
		//physics.Integrate( time.FixedStep() );
		sceneManager.FixedUpdate();
		time.SubtractFromPhysicsAccumulator( time.FixedStepActual() );
	}

	input.PostUpdate();
	sound.Update();

	//sceneManager.Render();
	newGraphics.DrawFrame();

	return 0;
}

//If the System is shutting down, that means end of program, time to do cleanup
void System::Shutdown( void )
{
	sceneManager.Shutdown();
	assets.CleanUp();
	sound.CleanUp();
	newGraphics.CleanUp();

	//Clean Up
    glfwDestroyWindow(window);

    glfwTerminate();
}

void System::Quit( void )
{
	glfwSetWindowShouldClose( window, GL_TRUE );
}

void System::key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	System* sys = static_cast<System*>( glfwGetWindowUserPointer( window ) );

	sys->input.ReceiveKeyboardInput( key, scancode, action, mods );
}

/*
		case WM_MOVE: //Remember new position //CLIENT AREA
			{
				int xPos = ( int )LOWORD( lParam );
				int yPos = ( int )HIWORD( lParam );
				window.SetClientPosition( xPos, yPos );
				break;
			}
		case WM_MOVING:	//Remember new position //WINDOW AREA
			{
				RECT* temp = (RECT*)lParam;
				window.SetWindowPosition( ( temp->left ), ( temp->top ) );
				return true;
			}
//*/