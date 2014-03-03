//Graphics Class
//Initialise DirectX, handle drawing calls

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include "Graphics/VertexFormats.h"
#include "Graphics/dMode.h"
#include "Graphics/adapter.h"
#include "Logging/Log.h"

#include <vector>

class System;
class Settings;
class iWindow;

enum ASPECT { FourThree = 43, SixteenNine = 169, SixteenTen = 1610 };
enum BUFFER { ThirtyTwo = 21, TwentyFour = 22, Sixteen = 23 };
enum DEPTH { D32 = 71, D24S8 = 75, D15S1 = 73, D16 = 80 };

class Graphics
{
private:
protected:

	Log log;

	System& system;
	Settings& settings;
	iWindow& window;

	LPDIRECT3D9 mInterface;						//interface to DirectX
	LPDIRECT3DDEVICE9 mDevice;					//the rendering device (graphics card)
	D3DPRESENT_PARAMETERS* presentParameters;	//Device info
	D3DPRESENT_PARAMETERS fullParameters;
	D3DPRESENT_PARAMETERS windowParameters;

	D3DCAPS9 caps;

	//Setup
	UINT adapterCount, modeCount;
	UINT adapter, mode;
	D3DDEVTYPE deviceType;
	D3DFORMAT backbufferFormat, depthFormat;
	DWORD behaviourFlags, qualityAA;
	int bufferCount;
	D3DMULTISAMPLE_TYPE AA;
	D3DDISPLAYMODE* modes;
	std::vector<dMode> dModes;
	std::vector<adapterDesc> adapterList;
	UINT refresh;
	ASPECT aspect;

	HRESULT deviceState;

	bool fullscreen;
	int xResolution, yResolution;
	
	bool showCursorFullscreen;
	
	void CheckDeviceCaps( void );
	void GetAdaptersFromInterface( void );
	void GetModesFromInterface( void );
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

	void SelectAdapter( const int& index );
	void SelectBufferFormat( const int& index );
	void SelectDepthFormat( const int& index );
	void SelectMultisample( const int& samples );
	void SelectAspect( const int& index );
	void SelectResolution( const int& index );

	void SetClientSize( const int& width, const int& height );

	void CreateInterface( void );
	void CreateDevice( void );

	void CheckDevice( void );
	bool Reset( void );

	void ReadSettings( void );
	void WriteSettings( void );
	void ApplySettings( void );

	void SetFullscreen( void );
	void SetWindowed( void );
public:

	Graphics( System& s );
	~Graphics( void );

	LPDIRECT3D9 const Interface( void ) const;
	LPDIRECT3DDEVICE9 const Device( void ) const;

	bool IsDeviceLost( void );

	void Init( void );
	void CleanUp( void );

	void SetDebugStates( void );

	void Refresh( void );

	//Menu Options
	const std::vector<adapterDesc>& GetAdapters( void );
	const std::vector<dMode>& GetDisplayModes( void );

	void SetResolution( const int& width, const int& height );
	void ToggleFullscreen( void );

	void ErrorCheck( HRESULT result, const char* const info );
};

#endif //_GRAPHICS_H_