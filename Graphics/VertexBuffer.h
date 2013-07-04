#ifndef _VERTEXBUFFER_H_
#define _VERTEXBUFFER_H_

#include "Graphics\Graphics.h"

template <class T>
class VertexBuffer
{
private:
protected:

	Graphics* const graphics;
	LPDIRECT3DVERTEXBUFFER9 buffer;

	DWORD usage, format;
	D3DPOOL pool;
public:

	VOID* memory;
	T* vertices;

	VertexBuffer( void );
	~VertexBuffer( void );

	LPDIRECT3DVERTEXBUFFER9 getBuffer( void );

	void Create( void );
	void Destroy( void );
	void OnLost( void );
	void OnRecover( void );

	void Lock( void );
	void Unlock( void );
	void CopyToBuffer( void );
};

#include "Graphics\VertexBuffer.cpp"

#endif //_VERTEXBUFFER_H_