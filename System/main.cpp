#include <stdexcept>
#include <memory>

#include "System/System.h"
#include "Window/WindowGLFW.h"
#include "Graphics/GraphicsVK.h"
#include "Sound/Sound.h"
#include "Input/Input.h"

#include "Graphics/DisableSteamOverlay.h"

int main()
{
    WindowGLFW window{};
    GraphicsVK graphics{};
    Sound sound{};
    Input input{};

    std::unique_ptr<System> sys;

    int result = 0;

    try
    {
        HandleWin32(); //TODO: Rename this to HandlePlatformSpecific?

        graphics.Window = window.GetWindow();

        sys = std::make_unique<System>( window, graphics, sound, input );

        if( !sys->Init() )
        {
            throw std::runtime_error( "Error during initialisation" );
        }

        //Run() is the main loop
        result = sys->Run();

        sys->Shutdown();
    }
    catch( std::exception error )
    {
        //TODO: Alert the user
        if( sys ){ sys->Quit(); }
    }
    catch( std::string error )
    {
        //TODO: Alert the user
        if( sys ){ sys->Quit(); }
    }

    return result;
}