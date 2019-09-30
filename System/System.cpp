#define settingsFile "w:/engine/data/settings/MainSettings.json"

#include "System/System.h"
#include "Window/WindowGLFW.h"
#include "Graphics/GraphicsVK.h"

System::System( WindowGLFW& Window,	GraphicsVK& Graphics, Sound& Sound,	Input& Input )
: Window( Window ), Graphics( Graphics ), Sound( Sound ), Input( Input )
{
	TRACE( "System Init" );

	std::ifstream mainSettingsFile{ settingsFile };
	
	mainSettingsFile >> GlobalSettings;

	Graphics.Init();
	Sound.Init();
	Input.Init();

	SetupCallbacks();

	//assets?
	
	sceneManager.Init();
}

System::~System( void )
{
	sceneManager.Shutdown();
	assets.CleanUp();
	sound.CleanUp();
	Graphics->CleanUp();

	std::ofstream mainSettingsFile{ settingsFile };

	mainSettingsFile << std::setw(4) << GlobalSettings << std::endl;
}

int System::Run( void )
{
	while( Window->IsValid() )
    {
    	time.FrameBegin();

		GameLoop();

		time.FrameEnd();
    }

    return 0;
}

int System::GameLoop( void )
{
	time.AddToPhysicsAccumulator( time.DeltaTimeActual() );

	sceneManager.Prepare();

	Window->Update();
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
	sound.Update(); //Why is this here?

	Graphics->Begin();

	//sceneManager.Render();

	Graphics->End();

	Graphics->Submit();

	return 0;
}

void System::Quit( void )
{
	Window->Quit();
}

void System::SetupCallbacks( void )
{
	auto key_callback = [&input]( GLFWwindow* Window, int Key, int Scancode, int Action, int Mods )
    {
        input.ReceiveKeyboardInput( Key, Scancode, Action, Mods );
    };

    auto cursor_position_callback = [&input]( GLFWwindow* Window, double X, double Y )
    {
        input.ReceiveMousePosition( X, Y );
    }

    auto mouse_button_callback = [&input]( GLFWwindow* Window, int Button, int Action, int Mods )
    {
        input.ReceiveMouseInput( Button, Action, Mods );
    }

    auto scroll_callback = [&input]( GLFWwindow* Window, double XOffset, double YOffset )
    {
        input.ReceiveScrollInput( XOffset, YOffset );
    }

    auto scroll_callback = [&input]( GLFWwindow* Window, double XOffset, double YOffset )
    {
        input.ReceiveScrollInput( XOffset, YOffset );
    }

    auto window_size_callback = [&input]( GLFWwindow* Window, int Width, int Height )
    {
        //Record in settings?
    }

    auto framebuffer_size_callback = [&input]( GLFWwindow* Window, int Width, int Height )
    {
        //Update graphics service + record in settings
        //glViewport(0, 0, width, height);
    }

    glfwSetKeyCallback( window.GetWindow(), key_callback );
    glfwSetCursorPosCallback( window.GetWindow(), cursor_position_callback );
    glfwSetMouseButtonCallback( window.GetWindow(), mouse_button_callback );
    glfwSetScrollCallback( window.GetWindow(), scroll_callback );
    glfwSetWindowSizeCallback( window.GetWindow(), window_size_callback );
    glfwSetFramebufferSizeCallback( window.GetWindow(), framebuffer_size_callback );
}