#include <stdexcept>
#include <memory>

#include "System/System.h"
#include "Window/WindowGLFW.h"
#include "Graphics/GraphicsVK.h"
#include "Sound/Sound.h"
#include "Input/Input.h"

#include "Graphics/DisableSteamOverlay.h"

void SetupInputCallbacks( WindowGLFW& window, Input& input );
void SetupRenderCallbacks( WindowGLFW& window, GraphicsVK& graphics );

int main()
{
    HandleWin32(); //TODO: Rename this to HandlePlatformSpecific?
    
    std::unique_ptr<System> sys;

    int result = 0;

    try
    {
        WindowGLFW window{};
        GraphicsVK graphics{ window.GetWindow() };
        Sound sound{};
        Input input{};

        SetupInputCallbacks( window, input );
        SetupRenderCallbacks( window, graphics );

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

void SetupInputCallbacks( WindowGLFW& window, Input& input )
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

    glfwSetKeyCallback( window.GetWindow(), key_callback );
    glfwSetCursorPosCallback( window.GetWindow(), cursor_position_callback );
    glfwSetMouseButtonCallback( window.GetWindow(), mouse_button_callback );
    glfwSetScrollCallback( window.GetWindow(), scroll_callback );
}

void SetupRenderCallbacks( WindowGLFW& window, GraphicsVK& graphics )
{
    auto window_size_callback = [&graphics]( GLFWwindow* Window, int Width, int Height )
    {
        //Record in settings?
    }

    auto framebuffer_size_callback = [&graphics]( GLFWwindow* Window, int Width, int Height )
    {
        //Update graphics service + record in settings
        //glViewport(0, 0, width, height);
    }

    glfwSetWindowSizeCallback( window.GetWindow(), window_size_callback );
    glfwSetFramebufferSizeCallback( window.GetWindow(), framebuffer_size_callback );
}