#include "Sprite/AnimatedSpriteProcessor.h"
#include "Graphics/Graphics.h"
#include "Transform/TransformProcessor.h"

#include <assert.h>

AnimatedSpriteProcessor::AnimatedSpriteProcessor( Graphics& g, TransformProcessor& t ) : graphics( g ), transforms( t ),
																		vBuffer( NULL ),
																		iBuffer( NULL ),
																		vDecl( NULL ),
																		numVerts( 4 ),
																		posOffset( 0*sizeof( float ) ),
																		texOffset( 3*sizeof( float ) ),
																		stride( 5*sizeof( float ) ),
																		effect( NULL ),
																		effectFile( "W:/engine/code/Sprite/TestShader.fx" ),
																		shaderFlags( D3DXSHADER_USE_LEGACY_D3DX9_31_DLL ),
																		log( "AnimatedSpriteProcessor" )
{
	D3DVERTEXELEMENT9 pos = { 0, posOffset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 };
	D3DVERTEXELEMENT9 tex = { 0, texOffset, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 };
	D3DVERTEXELEMENT9 endElement = D3DDECL_END();
	
	vElements.push_back( pos );
	vElements.push_back( tex );
	vElements.push_back( endElement );
	
	graphics.Device()->CreateVertexDeclaration( &vElements[0], &vDecl );
	
	VFormat vertices[] =
	{
		{ -0.5f, 0.5f, 0.0f, 0.0f, 0.0f },
		{ 0.5f, 0.5f, 0.0f, 1.0f, 0.0f },
		{ -0.5f, -0.5f, 0.0f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f, 1.0f }
	};

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
	"Error creating Sprite Shader" );
	
	worldHandle = effect->GetParameterBySemantic( NULL, "WORLD" );
	viewHandle = effect->GetParameterBySemantic( NULL, "VIEW" );
	projHandle = effect->GetParameterBySemantic( NULL, "PROJECTION" );
	texTransHandle = effect->GetParameterBySemantic( NULL, "TEXTRANSFROM" );
	textureHandle = effect->GetParameterByName( NULL, "g_MeshTexture" );

	//TODO: Switch back to SpriteShader.fx	
	RenderWithTexture = effect->GetTechniqueByName( "RenderSpriteWithTexture" );

	D3DXMatrixIdentity( &viewMat );
	D3DXMatrixIdentity( &projMat );

	D3DXMatrixLookAtLH( &viewMat, &(D3DXVECTOR3( 0.0f, 0.0f, -1.0f )), &(D3DXVECTOR3( 0.0f, 0.0f, 0.0f )), &(D3DXVECTOR3( 0.0f, 1.0f, 0.0f )) );
	
	//TODO: Get screen info from Graphics class(?) - This is a good reason to abstract this code away into the graphics class
	D3DXMatrixOrthoLH( &projMat, 1280, 720, 0.0f, 1.0f );

	//Set View Matrix
	effect->SetMatrix( viewHandle, &viewMat );
	effect->SetMatrix( projHandle, &projMat );
}

AnimatedSpriteProcessor::~AnimatedSpriteProcessor( void )
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

void AnimatedSpriteProcessor::AddAnimatedSpriteComponent( const unsigned int entityID )
{
	AnimatedSpriteComponent s;

	spriteComponents[entityID] = s;
}

AnimatedSpriteComponent& AnimatedSpriteProcessor::GetAnimatedSpriteComponent( const unsigned int entityID )
{
	return spriteComponents[ entityID ];
}

void AnimatedSpriteProcessor::Start( void )
{
}
#include <string>
void AnimatedSpriteProcessor::DrawSprite( const unsigned int entityID, const AnimatedSpriteComponent& sprite )
{
	D3DXMATRIX world, textureTransformation; //Passed to shader
	D3DXMATRIX translation, localRotationX, localRotationY, localRotationZ, scaling; //Geometry
	D3DXMATRIX textureScaling, textureTranslation; //Texture Coordinates

	D3DXMatrixIdentity( &world ); D3DXMatrixIdentity( &textureTransformation );
	D3DXMatrixIdentity( &translation );
	D3DXMatrixIdentity( &localRotationX ); D3DXMatrixIdentity( &localRotationY ); D3DXMatrixIdentity( &localRotationZ );
	D3DXMatrixIdentity( &scaling );
	D3DXMatrixIdentity( &textureScaling ); D3DXMatrixIdentity( &textureTranslation );

	const TransformComponent& tForm = transforms.GetTransformComponent( entityID );
	const Frame& currentFrame = sprite.GetCurrentAnimation().GetCurrentFrame();

	D3DXMatrixTranslation( &translation, tForm.translation.x, tForm.translation.y, tForm.translation.z );

	D3DXMatrixRotationX( &localRotationX, tForm.localRotation.x );
	D3DXMatrixRotationY( &localRotationY, tForm.localRotation.y );
	D3DXMatrixRotationZ( &localRotationZ, tForm.localRotation.z );
	
	D3DXMatrixScaling( &world, tForm.scale.x, tForm.scale.y, 1 );

	//Multplication order: scaling * localRotation * translation * rotation
	//TODO: Implement non-local rotations?
	world = world * scaling * localRotationX * localRotationY * localRotationZ * translation;

	effect->SetMatrix( worldHandle, &world );
	
	effect->SetTexture( textureHandle, currentFrame.Texture );

	//Hardcoded texture coordinates have height/length of 1
	//Scaling amount equals new height/width divided by the old (1.0f)
	float heightScaling = ( currentFrame.TextureDimensions.bottom - currentFrame.TextureDimensions.top );
	float widthScaling = ( currentFrame.TextureDimensions.right - currentFrame.TextureDimensions.left );

	//This transforms the texture coordinates that are hard coded in the vertex array
	D3DXMatrixScaling( &textureScaling, widthScaling, heightScaling, 1 );
	//D3DXMatrixRotationZ( &textureRotation, currentFrame.TextureRotation );
	D3DXMatrixTranslation( &textureTranslation, currentFrame.TextureDimensions.left, currentFrame.TextureDimensions.top, 0 );
	
	textureTransformation = textureScaling * textureTranslation;

	effect->SetMatrix( texTransHandle, &textureTransformation );
	
	//TODO: Switch effect file
	effect->SetTechnique( RenderWithTexture );
	
	//Apply the technique contained in the effect 
	UINT cPasses = 0;
	effect->Begin( &cPasses, 0 );

	for( int iPass = 0; iPass < cPasses; iPass++ )
	{
		effect->BeginPass( iPass );

		//Render the geometry with the applied technique
		graphics.ErrorCheck( graphics.Device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 ), "Drawing sprite" );

		effect->EndPass();
	}
	
	effect->End();
	
	graphics.ErrorCheck( graphics.Device()->SetStreamSource( 0, NULL, 0, 0 ), "Clearing stream source" );
	graphics.ErrorCheck( graphics.Device()->SetIndices( NULL ), "Clearing indices" );
}

void AnimatedSpriteProcessor::Update( const EngineDuration& deltaT )
{
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE ), "Setting sprite render state" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_ZWRITEENABLE, D3DZB_FALSE ), "Disabling Z-writes" );
	//graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_SRGBWRITEENABLE, TRUE ), "Setting sprite render state" );

	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_LIGHTING, false ), "Disabling Lighting Failed" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ), "Disabling Culling" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE ), "Disabling Depth Testing" );

	graphics.ErrorCheck( graphics.Device()->SetVertexDeclaration( vDecl ), "Setting vertex declaration" );
	graphics.ErrorCheck( graphics.Device()->SetStreamSource( 0, vBuffer, 0, stride ), "Setting stream source" );
	graphics.ErrorCheck( graphics.Device()->SetIndices( iBuffer ), "Setting indices" );

	ListOfAnimatedSprites::iterator it;

	for( it = spriteComponents.begin(); it != spriteComponents.end(); it++ )
	{
		it->second.Update( deltaT );
		
		DrawSprite( it->first, it->second );
	}
	
	graphics.Device()->SetRenderState( D3DRS_ZWRITEENABLE, D3DZB_TRUE );
	graphics.Device()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
}

void AnimatedSpriteProcessor::End( void )
{
}