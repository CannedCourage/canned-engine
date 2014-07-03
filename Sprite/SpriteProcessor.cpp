#include "Sprite/SpriteProcessor.h"
#include "Graphics/Graphics.h"

#include <assert.h>

SpriteProcessor::SpriteProcessor( Graphics& g ) : 	graphics( g ),
													vBuffer( NULL ),
													iBuffer( NULL ),
													vDecl( NULL ),
													numVerts( 4 ),
													posOffset( 0*sizeof( float ) ),
													texOffset( 3*sizeof( float ) ),
													stride( 5*sizeof( float ) ),
													effect( NULL ),
													effectFile( "C:/Users/Scott/Programming/Projects/GitHub/canned-engine/Sprite/TestShader.fx" ),
													shaderFlags( D3DXSHADER_USE_LEGACY_D3DX9_31_DLL ),
													log( "SpriteProcessor" )
{
	D3DVERTEXELEMENT9 pos = { 0, posOffset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 };
	D3DVERTEXELEMENT9 tex = { 0, texOffset, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 };
	D3DVERTEXELEMENT9 endElement = D3DDECL_END();
	
	vElements.push_back( pos );
	vElements.push_back( tex );
	vElements.push_back( endElement );
	
	graphics.Device()->CreateVertexDeclaration( &vElements[0], &vDecl );
	
	VFormat vertices[] =
	{ { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f, 1.0f } };

	verts.push_back( vertices[0] );
	verts.push_back( vertices[1] );
	verts.push_back( vertices[2] );
	verts.push_back( vertices[3] );

	//Create non-FVF vertex buffer
	graphics.ErrorCheck( graphics.Device()->CreateVertexBuffer( 20*sizeof(float), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vBuffer, NULL ), "Error creating sprite buffer" );
	
	BYTE* vertexData;
	graphics.ErrorCheck( vBuffer->Lock( 0, 0, reinterpret_cast<void**>( &vertexData ), 0 ), "Error locking vertex buffer" );
	memcpy( vertexData, vertices, 20*sizeof(float) );
	graphics.ErrorCheck( vBuffer->Unlock(), "Error unlocking vertex buffer" );
	
	indicies.push_back( 0 );
	indicies.push_back( 1 );
	indicies.push_back( 2 );
	indicies.push_back( 2 );
	indicies.push_back( 1 );
	indicies.push_back( 3 );

	// build index buffer
	graphics.ErrorCheck( graphics.Device()->CreateIndexBuffer( indicies.size(), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &iBuffer, NULL ), "Error creating sprite index buffer" );

	BYTE* indexData;
	graphics.ErrorCheck( iBuffer->Lock( 0, 0, reinterpret_cast<void**>( &indexData ), 0 ), "Error locking index buffer" );
	memcpy( indexData, &indicies[0], indicies.size() );
	graphics.ErrorCheck( iBuffer->Unlock(), "Error unlocking index buffer" );

	graphics.ErrorCheck(
	D3DXCreateEffectFromFile( 
    graphics.Device(), 
    effectFile, 
    NULL, // CONST D3DXMACRO* pDefines,
    NULL, // LPD3DXINCLUDE pInclude,
    shaderFlags, 
	NULL, // LPD3DXEFFECTPOOL pPool,
    &effect, 
    NULL ),
	"Creating Sprite Shader" );
	
	worldHandle = effect->GetParameterBySemantic( NULL, "WORLD" );
	viewHandle = effect->GetParameterBySemantic( NULL, "VIEW" );
	projHandle = effect->GetParameterBySemantic( NULL, "PROJECTION" );
	//texTransHandle = effect->GetParameterByName( NULL, "matTex" );
	textureHandle = effect->GetParameterByName( NULL, "g_MeshTexture" );
	//RenderWithTexture = effect->GetTechniqueByName( "RenderSpriteWithTexture" );
	RenderWithoutTexture = effect->GetTechniqueByName( "RenderSpriteWithoutTexture" );
}

SpriteProcessor::~SpriteProcessor( void )
{
	if( vDecl != NULL )
	{
		vDecl->Release();
		vDecl = NULL;
	}
	
	if( vBuffer != NULL )
	{
		vBuffer->Release();
		vBuffer = NULL;
	}
	
	if( iBuffer != NULL )
	{
		iBuffer->Release();
		iBuffer = NULL;
	}
	
	if( effect != NULL )
	{
		effect->Release();
		effect = NULL;
	}
}

void SpriteProcessor::AddSpriteComponent( const unsigned int entityID, IDirect3DTexture9* texture )
{
	SpriteComponent s;

	s.texture = texture;

	spriteComponents[ entityID ] = s;
}

SpriteComponent& SpriteProcessor::GetSpriteComponent( const unsigned int entityID )
{
	return spriteComponents[ entityID ];
}

void SpriteProcessor::Start( void )
{
}

void SpriteProcessor::Update( const double& deltaT )
{
	//graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE ), "Setting sprite render state" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_ZWRITEENABLE, D3DZB_FALSE ), "Setting sprite render state" );
	//graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_SRGBWRITEENABLE, TRUE ), "Setting sprite render state" );

	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_LIGHTING, false ), "Setting Lighting State Failed" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ), "Setting Culling Mode Failed" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE ), "Disabling Depth Testing" );
	//graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_COLORVERTEX, TRUE ), "Enabling Vertex Colours" );

	graphics.ErrorCheck( graphics.Device()->SetVertexDeclaration( vDecl ), "Setting vertex declaration" );
	graphics.ErrorCheck( graphics.Device()->SetStreamSource( 0, vBuffer, 0, stride ), "Setting stream source" );
	graphics.ErrorCheck( graphics.Device()->SetIndices( iBuffer ), "Setting indices" );

	D3DXMATRIX worldMat, viewMat, projMat, texTransMat;

	D3DXMatrixIdentity( &worldMat );
	D3DXMatrixIdentity( &viewMat );
	D3DXMatrixIdentity( &projMat );
	D3DXMatrixIdentity( &texTransMat );

	D3DXMatrixTranslation( &worldMat, 0, 0, 0 );
	D3DXMatrixScaling( &worldMat, 640, 360, 0 );

	D3DXMatrixLookAtLH( &viewMat, &(D3DXVECTOR3( 0.0f, 0.0f, -1.0f )), &(D3DXVECTOR3( 0.0f, 0.0f, 0.0f )), &(D3DXVECTOR3( 0.0f, 1.0f, 0.0f )) );
	
	D3DXMatrixOrthoLH( &projMat, 1280, 720, 0.0f, 1.0f );

	std::map<int, SpriteComponent>::iterator it;
	//*
	for( it = spriteComponents.begin(); it != spriteComponents.end(); it++ )
	{
		//Set World Matrix - This will come from Transform
		effect->SetMatrix( worldHandle, &worldMat ); //Calculate from Transform

		//Set View Matrix
		effect->SetMatrix( viewHandle, &viewMat );
		effect->SetMatrix( projHandle, &projMat );
		
		effect->SetTexture( textureHandle, it->second.texture ); //From sprite component

		//Set Texture Matrix (in shader)
		//effect->SetMatrix( texTransHandle, &texTransMat ); //Calculate from texture coordinates in sprite component
		
		effect->SetTechnique( RenderWithoutTexture );
		
		// Apply the technique contained in the effect 
		UINT cPasses = 0;
		effect->Begin(&cPasses, 0);

		for(int iPass = 0; iPass < cPasses; iPass++)
		{
			effect->BeginPass(iPass);

			// Only call CommitChanges if any state changes have happened
			// after BeginPass is called
			//effect->CommitChanges();

			// Render the mesh with the applied technique
			//graphics.ErrorCheck( graphics.Device()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, numVerts, 0, 2 ), "Drawing sprite" );
			graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 ), "Drawing cube" );

			effect->EndPass();
		}
		
		effect->End();
		
		graphics.Device()->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
		graphics.ErrorCheck( graphics.Device()->SetStreamSource( 0, NULL, 0, 0 ), "Clearing stream source" );
		graphics.ErrorCheck( graphics.Device()->SetIndices( NULL ), "Clearing indices" );
	}
	//*/
	/*
	for( it = spriteComponents.begin(); it != spriteComponents.end(); it++ )
	{
		graphics.Device()->SetTransform( D3DTS_VIEW, &viewMat );
		graphics.Device()->SetTransform( D3DTS_WORLD, &worldMat );
		graphics.Device()->SetTransform( D3DTS_PROJECTION, &projMat );

		graphics.Device()->SetTexture( 0, it->second.texture );

		//graphics.ErrorCheck( graphics.Device()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, numVerts, 0, 2 ), "Drawing sprite" );
		graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 ), "Drawing cube" );
	}
	//*/
	graphics.Device()->SetRenderState( D3DRS_ZWRITEENABLE, D3DZB_TRUE );
	graphics.Device()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
}

void SpriteProcessor::End( void )
{
}