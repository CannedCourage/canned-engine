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
    std::unique_ptr<System> sys;

    int result = 0;

    try
    {
        WindowGLFW window{};
        GraphicsVK graphics{ window.GetWindow() };
        Sound sound{};
        Input input{};
        
        HandleWin32(); //TODO: Rename this to HandlePlatformSpecific?

        sys = std::make_unique<System>( window, graphics, sound, input );

        //Run() is the main loop
        result = sys->Run();
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