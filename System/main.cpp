#include "System/System.h"

#include <stdexcept> 

System* sys = NULL;

int main()
{
    int result = 0;

    try
    {
        sys = new System();
        
        if( !sys )
        {
            throw std::runtime_error( "Error during construction" );
        }

        if( !sys->Initialise() )
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
        if( sys != NULL )
        {
            sys->Quit();
        }
    }
    catch( std::string error )
    {
        //TODO: Alert the user
        if( sys != NULL )
        {
            sys->Quit();
        }
    }

    if( sys != NULL )
    {
        delete sys;
        sys = NULL;
    }

    return result;
}