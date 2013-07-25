#include "Window/iWindow.h"
#include "System/System.h"

Window( System &s ) : 	log( "Window" ), settings( s.settings ), maximised( false ), 
						windowX( 0 ), windowY( 0 ), windowWidth( 0 ), windowHeight( 0 ), 
						clientX( 0 ), clientY( 0 ), clientWidth( 0 ), clientHeight( 0 )
{
}