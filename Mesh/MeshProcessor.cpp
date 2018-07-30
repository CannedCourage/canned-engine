#include "Mesh/MeshProcessor.h"

MeshProcessor::MeshProcessor( Graphics& g ) : log( "MeshProcessor" ), graphics( g )
{
}

MeshProcessor::~MeshProcessor( void )
{
}

void MeshProcessor::AddMeshComponent( const unsigned int entityID, const Mesh& meshAsset )
{
	meshComponents[ entityID ] = meshAsset;
}

MeshComponent& MeshProcessor::GetMeshComponent( const unsigned int entityID )
{
	return meshComponents[ entityID ];
}

void MeshProcessor::Start( void )
{
}

void MeshProcessor::End( void )
{
}

void MeshProcessor::Update( const EngineDuration& deltaT )
{
	/*
	graphics.Device()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	graphics.Device()->SetRenderState( D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	graphics.Device()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE);
	graphics.Device()->SetRenderState( D3DRS_SRGBWRITEENABLE, TRUE );

	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_LIGHTING, false ), "Setting Lighting State Failed" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW ), "Setting Culling Mode Failed" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE ), "Enabling Depth Testing" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_COLORVERTEX, TRUE ), "Enabling Vertex Colours" );

	std::map<int, MeshComponent>::iterator it;

	for( it = meshComponents.begin(); it != meshComponents.end(); it++ )
	{
		TRACE( "Rendering mesh" );
		
		const MeshComponent& mesh = it->second; //meshComponents[m];

		if( mesh.effect != NULL )
		{
			/*
			// set dynamic shader parameters
			{
				Math::Matrix viewProjection = graphics.GetViewMatrix() * graphics.GetProjectionMatrix();
				graphics.ErrorCheck( mesh.effect->SetMatrix( mesh.EffectParameters.viewProjection, &viewProjection.data ) );
		
				PoseBufferInterface& poseBuffer = *mPoseBuffers[mCurrentAnimationMethod];
				const float* poseData =  reinterpret_cast<const float*>( poseBuffer[0] );
				graphics.ErrorCheck( mesh.effect->SetFloatArray( mesh.EffectParameters.boneTransforms, poseData, poseBuffer.Size() / sizeof(float) ) );
			
				graphics.ErrorCheck( mesh.effect->SetInt( mesh.EffectParameters.mShaderTest, mShaderTest ) );
				graphics.ErrorCheck( mesh.effect->SetTechnique( mesh.EffectParameters.mTechniques[mCurrentAnimationMethod] ) );
			}
			//*/
			/*
			// set geometry data
			{
				graphics.ErrorCheck( graphics.Device()->SetVertexDeclaration( mesh.vertexDeclaration ), "Setting vertex declaration" );
				graphics.ErrorCheck( graphics.Device()->SetStreamSource( 0, mesh.vertexBuffer, 0, mesh.data.vertexSize ), "Setting stream source" );
				graphics.ErrorCheck( graphics.Device()->SetIndices( mesh.indexBuffer ), "Setting indices" );
			}

			// now render
			{
				//UINT cPasses;
				//graphics.ErrorCheck( mesh.effect->Begin( &cPasses, 0 ) );
				//for( unsigned int iPass = 0; iPass < cPasses; iPass++ )
				//{
					//graphics.ErrorCheck( mesh.effect->BeginPass( iPass ) );
					graphics.ErrorCheck( graphics.Device()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, mesh.data.vertexCount, 0, mesh.data.triangleCount ), "Drawing mesh" );
					//graphics.ErrorCheck( mesh.effect->EndPass() );
				//}
				//graphics.ErrorCheck( mesh.effect->End() );
			}
		}
	}
	
	graphics.Device()->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	graphics.ErrorCheck( graphics.Device()->SetStreamSource( 0, NULL, 0, 0 ), "Clearing stream source" );
	graphics.ErrorCheck( graphics.Device()->SetIndices( NULL ), "Clearing indices" );
	//*/
}