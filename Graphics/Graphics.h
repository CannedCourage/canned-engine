//Graphics Class
//Initialise DirectX, handle drawing calls

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include "Graphics\VertexFormats.h"
#include "Graphics\Screen.h"
#include "Logging\ILoggable.h"
#include "Engine\DefaultSettings.h"
#include "Window\Window.h"

class System;

enum ASPECT { FourThree, SixteenNine, SixteenTen };

class Graphics : public ILoggable
{
private:
protected:

	System& system;
	DefaultSettings &settings;
	Window &window;

	LPDIRECT3D9 mInterface;						//interface to DirectX
	LPDIRECT3DDEVICE9 mDevice;					//the rendering device (graphics card)
	D3DPRESENT_PARAMETERS* presentParameters;	//Device info
	D3DPRESENT_PARAMETERS fullParameters;
	D3DPRESENT_PARAMETERS windowParameters;

	D3DCAPS9 caps;

	//Setup
	UINT adapterCount, modeCount;
	UINT adapter, mode;
	D3DADAPTER_IDENTIFIER9* adapters;
	D3DDEVTYPE deviceType;
	D3DFORMAT backbufferFormat, depthFormat;
	DWORD behaviourFlags, qualityAA;
	int bufferCount;
	D3DMULTISAMPLE_TYPE AA;
	D3DDISPLAYMODE* modes;
	UINT refresh;
	ASPECT aspect;

	HRESULT deviceState;

	bool windowed;
	int xResolution, yResolution;
	int xWindowPosition, yWindowPosition;
	int xWindowSize, yWindowSize;
	int xClientPos, yClientPos;
	int clientWidth, clientHeight;
	
	bool showCursorFullscreen;
	bool forceClientToRes;
	void CheckDeviceCaps( void );
	void GetAdapters( void );
	void GetMode( void );
	void SetParameters( void );
	void SetBufferFormat16( void );
	void SetBufferFormat24( void );
	void SetBufferFormat32( void );
	void DoubleBuffer( void );
	void TripleBuffer( void );
	void SetDepthBufferD32( void );
	void SetDepthBufferD24S8( void );
	void SetDepthBufferD15S1( void );
	void SetDepthBufferD16( void );
	void SetMultisampleOff( void );
	void SetMultisample2( void );
	void SetMultisample4( void );
	void SetMultisample8( void );
	void SetMultisample16( void );
	void SelectAspect43( void );
	void SelectAspect169( void );
	void SelectAspect1610( void );

	void CheckDevice( void );
	void ForceWindowAroundClient( void );
public:

	Graphics( System& s );
	~Graphics( void );

	LPDIRECT3D9 const Interface( void ) const;
	LPDIRECT3DDEVICE9 const Device( void ) const;

	bool IsDeviceLost( void ) const;

	void Initialise( void );
	void CreateInterface( void );

	//Menu Options
	void SelectAdapter( int index );
	void SelectBufferFormat( const int& index );
	void SelectDepthFormat( const int& index );
	void SelectMultisample( const int& n );
	void SelectAspect( int index );
	void SelectResolution( const int& index );

	void CreateDevice( void );
	void SetRenderStates( void );
	void SetDebugStates( void );
	void SetStandardStates( void );
	void CleanUp( void );

	void DoChecks( void );
	bool Reset( void );

	//Mutators, change state information, but don't cause updates
	void SetResolution( const int& width, const int& height );
	void SetClientSize( const int& width, const int& height );
	void SetClientPosition( const int& x, const int& y );
	//Methods that cause updates immediately
	void SetFullscreen( void );
	void SetWindowed( void );
	void ChangeResolution( const int& width, const int& height );
	void ChangeView( void ); //Toggle Fullscreen
	void ChangeClientSize( const int& width, const int& height );

	void ErrorCheck( HRESULT result, LPCTSTR info );
};

#endif //_BALL_H_