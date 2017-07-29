// #ifndef _VERTEXBUFFER_H_
// #define _VERTEXBUFFER_H_

// #include <d3d9.h>
// #include <d3dx9.h>

// class System;
// class Graphics;

// template <class T>
// class VertexBuffer
// {
// private:
// protected:

// 	Graphics& graphics;
// 	LPDIRECT3DVERTEXBUFFER9 buffer;

// 	DWORD usage, format;
// 	D3DPOOL pool;
// public:

// 	VOID* memory;
// 	T* vertices;

// 	VertexBuffer( System &s );
// 	~VertexBuffer( void );

// 	LPDIRECT3DVERTEXBUFFER9 getBuffer( void );

// 	void Create( void );
// 	void Destroy( void );
// 	void OnLost( void );
// 	void OnRecover( void );

// 	void Lock( void );
// 	void Unlock( void );
// 	void CopyToBuffer( void );
// };

// #include "Graphics\VertexBuffer.cpp"

// #endif //_VERTEXBUFFER_H_