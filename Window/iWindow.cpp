#include "Window/iWindow.h"
#include "System/System.h"

iWindow::iWindow( System &s ) : 	log( "Window" ), settings( s.settings ), fullscreen( false ), 
						windowX( 0 ), windowY( 0 ), windowWidth( 0 ), windowHeight( 0 ), 
						clientX( 0 ), clientY( 0 ), clientWidth( 0 ), clientHeight( 0 )
{
	fullscreen = settings.GetBool( "display/fullscreen" );
}

iWindow::~iWindow( void )
{
	settings.SetInteger( "display/x", clientX );
	settings.SetInteger( "display/y", clientY );
}