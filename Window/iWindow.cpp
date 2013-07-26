#include "Window/iWindow.h"
#include "System/System.h"

iWindow::iWindow( System &s ) : 	log( "Window" ), settings( s.settings ), maximised( false ), 
						windowX( 0 ), windowY( 0 ), windowWidth( 0 ), windowHeight( 0 ), 
						clientX( 0 ), clientY( 0 ), clientWidth( 0 ), clientHeight( 0 )
{
	windowX = settings.window.x;
	windowY = settings.window.y;
	windowWidth = settings.window.width;
	windowHeight = settings.window.height;
	maximised = settings.window.maximised;
	clientX = settings.client.x;
	clientY = settings.client.y;
	clientWidth = settings.client.width;
	clientHeight = settings.client.height;
}

iWindow::~iWindow( void )
{

}