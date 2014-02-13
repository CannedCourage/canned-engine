//Graphics Class
//Initialise DirectX, handle drawing calls

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include "Graphics/VertexFormats.h"
#include "Logging/Log.h"

class System;
class Settings;
class WindowMS;

enum ASPECT { FourThree = 43, SixteenNine = 169, SixteenTen = 1610 };
enum BUFFER { ThirtyTwo, TwentyFour, Sixteen };
enum DEPTH { D32, D24S8, D15S1, D16 };
enum SAMPLE { Off, TwoSamples, FourSamples, EightSamples, SixteenSamples };

class Graphics
{
private:
protected:

	Log log;

	System& system;
	Settings& settings;
	WindowMS& window;

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

	bool fullscreen;
	int xResolution, yResolution;
	
	bool showCursorFullscreen;
	
	void CheckDeviceCaps( void );
	void GetAdapters( void );
	void GetModesFromDevice( void );
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

	void ForceWindowAroundClient( void );

	void CreateInterface( void );
public:

	Graphics( System& s );
	~Graphics( void );

	LPDIRECT3D9 const Interface( void ) const;
	LPDIRECT3DDEVICE9 const Device( void ) const;

	bool IsDeviceLost( void ) const;

	void Initialise( void );

	//Menu Options
	void SelectAdapter( const int& index );
	void SelectBufferFormat( const int& index );
	void SelectDepthFormat( const int& index );
	void SelectMultisample( const int& n );
	void SelectAspect( const int& index );
	void SelectResolution( const int& index );

	void CreateDevice( void );
	void SetDebugStates( void );
	void CleanUp( void );

	void CheckDevice( void );
	bool Reset( void );
	void Refresh( void );

	void ReadSettings( void );
	void WriteSettings( void );

	void SetResolution( const int& width, const int& height );
	void SetClientSize( const int& width, const int& height );
	void SetClientPosition( const int& x, const int& y );
	void SetFullscreen( void );
	void SetWindowed( void );
	void ToggleFullscreen( void );

	void ErrorCheck( HRESULT result, const char* const info );
};

#endif //_BALL_H_