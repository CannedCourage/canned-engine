#ifndef _DISABLESTEAMOVERLAY_H_
#define _DISABLESTEAMOVERLAY_H_

#ifdef _WIN32

#include <Windows.h>

void HandleWin32( void )
{
	// Steam's overlay is incompatible with LunarG's
	SetEnvironmentVariable( "DISABLE_VK_LAYER_VALVE_steam_overlay_1", "1" );
}

#endif //_WIN32

#endif //_DISABLESTEAMOVERLAY_H_