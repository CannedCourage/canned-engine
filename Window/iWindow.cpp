#include "Window/iWindow.h"
#include "System/System.h"

iWindow::iWindow( System &s ) : 	log( "Window" ), settings( s.settings ), fullscreen( false ), 
						windowX( 0 ), windowY( 0 ), windowWidth( 0 ), windowHeight( 0 ), 
						clientX( 0 ), clientY( 0 ), clientWidth( 0 ), clientHeight( 0 )
{
	windowX = settings.window.x;
	windowY = settings.window.y;
	windowWidth = settings.window.width;
	windowHeight = settings.window.height;
	fullscreen = settings.client.fullscreen;
	clientX = settings.client.x;
	clientY = settings.client.y;
	clientWidth = settings.client.width;
	clientHeight = settings.client.height;
}

iWindow::~iWindow( void )
{
	 settings.window.x = windowX;
	 settings.window.y = windowY;
	 settings.window.width = windowWidth;
	 settings.window.height = windowHeight;
	 //settings.client.fullscreen = fullscreen;
	 settings.client.x = clientX;
	 settings.client.y = clientY;
	 settings.client.width = clientWidth;
	 settings.client.height = clientHeight;
}