#define settingsFile "w:/engine/data/settings/MainSettings.json"

#include "Graphics/DisableSteamOverlay.h"

#include "StandardResources/resource.h"
#include "System/System.h"
#include "Window/WindowGLFW.h"
#include "Graphics/GraphicsVK.h"

System::System( void )
{
	std::ifstream mainSettingsFile{ settingsFile };
	
	mainSettingsFile >> GlobalSettings;

	HandleWin32(); //TODO: Rename this to HandlePlatformSpecific?

	//Instantiate services
	Window = std::make_unique<WindowGLFW>();
	Graphics = std::make_unique<GraphicsVK>();
}

System::~System( void )
{
	std::ofstream mainSettingsFile{ settingsFile };

	mainSettingsFile << std::setw(4) << GlobalSettings << std::endl;
}

int System::Initialise( void )
{
	TRACE( "System Init" );

	Window->Init( this );
	
	Graphics->Init();

	sound.Init();

	input.Init();

	//assets?
	
	sceneManager.Init();

	return 1;
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
	sound.Update();

	//sceneManager.Render();
	Graphics->DrawFrame();

	return 0;
}

//If the System is shutting down, that means end of program, time to do cleanup
void System::Shutdown( void )
{
	sceneManager.Shutdown();
	assets.CleanUp();
	sound.CleanUp();
	Graphics->CleanUp();
	Window->CleanUp();
}

void System::Quit( void )
{
	Window->Quit();
}