#ifndef _VBUFFERDEF_
#define _VBUFFERDEF_

#include "Graphics/VertexBuffer.h"
#include "System/System.h"

template <class T>
VertexBuffer<T>::VertexBuffer( System &s ) : graphics( s.graphics ), buffer( NULL ), usage( D3DUSAGE_WRITEONLY ), format( T::format ), pool( D3DPOOL_DEFAULT ), memory( NULL ), vertices( NULL )
{
}

template <class T>
VertexBuffer<T>::~VertexBuffer( void )
{
	Destroy();

	if( vertices != NULL )
	{
		delete[] vertices;
		vertices = NULL;
	}
}

template <class T>
LPDIRECT3DVERTEXBUFFER9 VertexBuffer<T>::getBuffer( void )
{
	return buffer;
}

template <class T>
void VertexBuffer<T>::Create( void )
{
	graphics.ErrorCheck( graphics.Device()->CreateVertexBuffer( sizeof(vertices), usage, format, pool, &buffer, NULL ), TEXT( "VertexBuffer creation failed" ) );
}

template <class T>
void VertexBuffer<T>::Destroy( void )
{
	if( buffer != NULL )
	{
		buffer->Release();
		buffer = NULL;
	}
}

template <class T>
void VertexBuffer<T>::OnLost( void )
{
	if( pool != D3DPOOL_MANAGED )
	{
		Destroy();
	}
}

template <class T>
void VertexBuffer<T>::OnRecover( void )
{
	Create();
	CopyToBuffer();
}

template <class T>
void VertexBuffer<T>::Lock( void )
{
	graphics.ErrorCheck( buffer->Lock( 0, 0, (void**)&memory, 0 ), TEXT( "Error locking vertex buffer buffer" ) );
}

template <class T>
void VertexBuffer<T>::Unlock( void )
{
	graphics.ErrorCheck( buffer->Unlock(), TEXT( "Error unlocking vertex buffer" ) );
}

template <class T>
void VertexBuffer<T>::CopyToBuffer( void )
{
	Lock();
	memcpy( memory, vertices, sizeof(vertices) );
	Unlock();
}

#endif //_VBUFFERDEF_