#include "Mesh/MeshManager.h"

#include <assert.h>

#include "Graphics\Graphics.h"

MeshManager::MeshManager( Graphics& g ) : log( "MeshManager" ), graphics( g )
{
}

MeshManager::~MeshManager( void )
{
	ReleaseResources();
}

bool MeshManager::LoadMesh( const char* file )
{
	Assimp::Importer modelImporter;
	const aiScene* scene = NULL;

	scene = modelImporter.ReadFile( file, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
												aiProcess_ConvertToLeftHanded | aiProcess_SortByPType );

	if( scene == NULL )
	{
		log.Message( modelImporter.GetErrorString() );

		return false;
	}
	else
	{
		BuildMesh( scene );

		return true;
	}
}

void MeshManager::GetVertexInformation( const aiMesh* mesh, MeshData* data )
{
	/*
	D3DVERTEXELEMENT9 {
    WORD Stream;      // This is the stream this element resides in
    WORD Offset;      // Offset from the beginning of the vertex data.  Get this by adding up the size of the types defined before this element.
    BYTE Type;        // The data type (ie D3DDECLTYPE_FLOAT3)
    BYTE Method;      // Use D3DDECLMETHOD_DEFAULT here
    BYTE Usage;       // Defines what the data will be used for (this is the semantic in your shader - ie D3DDECLUSAGE_POSITION)
    BYTE UsageIndex;  // Modifies the usage data to allow the user to specify multiple usage types.
	}
	 */
	
	//Add a vertex element for each type of information in the mesh, i.e. positons, normals, texture coordinates
	
	int offset = 0;

	if( mesh->HasPositions() )
	{
		D3DVERTEXELEMENT9 decl = { 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 };
		data->vertexElements.push_back( decl );
		offset += ( 3 * sizeof(float) );

		//Determine index size (16/32)
		data->indexFormat = mesh->mNumVertices < 0xFFFF ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
		data->indexSize = data->indexFormat == D3DFMT_INDEX16 ? sizeof(UINT16) : sizeof(UINT32);
	}

	//End vertex declaration
	D3DVERTEXELEMENT9 endElement = D3DDECL_END();

	data->vertexElements.push_back( endElement );

	data->vertexSize = offset;
	data->vertexCount = mesh->mNumVertices;
	data->triangleCount = mesh->mNumFaces;
}

void MeshManager::BuildMesh( const aiScene* sc )
{
	//For each mesh in scene
	int numMeshes = sc->mNumMeshes;

	for( int m = 0; m < numMeshes; m++ )
	{
		log.Message( "Building Mesh" );
		meshComponents.push_back( Mesh() );

		MeshData& data = meshComponents.back().data;

		//Get mesh
		const aiMesh* mesh = sc->mMeshes[m];
		
		GetVertexInformation( mesh, &data );

		int offset = 0;

		if( mesh->HasPositions() )
		{
			//Fill vector with vertex data
			const int vertexBufferSize = data.vertexCount * data.vertexSize;

			data.vertexData.resize( vertexBufferSize );

			for( unsigned int v = 0; v < data.vertexCount; ++v )
			{
				const aiVector3D& element = mesh->mVertices[v];

				float _data[] = { element[0], element[1], element[2] };

				memcpy( &data.vertexData[v * data.vertexSize] + offset, _data, 3 * sizeof(float) );
			}

			offset += 3 * sizeof(float);

			const int indexBufferSize = data.triangleCount * 3 * data.indexSize;
			data.indexData.resize( indexBufferSize );

			//For each face
			for( unsigned int f = 0; f < data.triangleCount; ++f )
			{
				const aiFace& face = mesh->mFaces[f];
				
				if( face.mNumIndices != 3 )
				{
					throw( "Mesh has non-triangular polygons" );
				}

				UINT32 indices[3];

				//Copy indices into array of 3 UINT16/32
				if( data.indexFormat == D3DFMT_INDEX16 )
				{
					assert( face.mIndices[0] < 0xFFFF && face.mIndices[1] < 0xFFFF && face.mIndices[2] < 0xFFFF );

					UINT16* indices16 = reinterpret_cast<UINT16*>( indices );
					indices16[0] = face.mIndices[0];
					indices16[1] = face.mIndices[1];
					indices16[2] = face.mIndices[2];
				}
				else
				{
					indices[0] = face.mIndices[0];
					indices[1] = face.mIndices[1];
					indices[2] = face.mIndices[2];
				}

				//memcpy into indices vector
				memcpy( &data.indexData[f * 3 * data.indexSize], indices, 3 * data.indexSize );
			}
		}
	}

	AccquireResources();
}

void MeshManager::AccquireResources( void )
{
	for( unsigned int m = 0; m < meshComponents.size(); ++m )
	{
		log.Message( "Accquiring Resources" );

		Mesh& mesh = meshComponents[m];

		graphics.Device()->CreateVertexDeclaration( &mesh.data.vertexElements[0], &mesh.vertexDeclaration );

		// now create vertex buffer
		graphics.ErrorCheck( graphics.Device()->CreateVertexBuffer( mesh.data.vertexData.size(), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &mesh.vertexBuffer, NULL ), "ERror creating vertex buffer" );

		BYTE* vertexData;
		graphics.ErrorCheck( mesh.vertexBuffer->Lock( 0, 0, reinterpret_cast<void**>( &vertexData ), 0 ), "Error locking vertex buffer" );
			memcpy( vertexData, &mesh.data.vertexData[0], mesh.data.vertexData.size() );
		graphics.ErrorCheck( mesh.vertexBuffer->Unlock(), "Error unlocking vertex buffer" );


		// build index buffer
		graphics.ErrorCheck( graphics.Device()->CreateIndexBuffer( mesh.data.indexData.size(), D3DUSAGE_WRITEONLY, mesh.data.indexFormat, D3DPOOL_DEFAULT, &mesh.indexBuffer, NULL ), "Error creating index buffer" );

		BYTE* indexData;
		graphics.ErrorCheck( mesh.indexBuffer->Lock( 0, 0, reinterpret_cast<void**>( &indexData ), 0 ), "Error locking index buffer" );
			memcpy( indexData, &mesh.data.indexData[0], mesh.data.indexData.size() );
		graphics.ErrorCheck( mesh.indexBuffer->Unlock(), "Error unlocking index buffer" );

		/*/ load shaders
		{
			LPD3DXBUFFER buf;	
			const char* shaderName = "../Shaders/Model.fx";
			HRESULT hr = D3DXCreateEffectFromFile( graphics.Device(), shaderName, NULL, NULL, NULL, NULL, &mesh.effect, &buf );

			if( FAILED(hr) && buf)
			{
				const char* errorText = reinterpret_cast<char*>( buf->GetBufferPointer() );
				char errorTitle[1024];
				sprintf_s( errorTitle, "Error compiling shader '%s'", shaderName );
				
				buf->Release();

				log.Message( errorTitle );
				throw( errorTitle );
			}
			else 
			{
				// set static shader parameters
				D3DXHANDLE hDiffuseMap = mesh.effect->GetParameterBySemantic( NULL, "DIFFUSE_MAP" );
				graphics.ErrorCheck( mesh.effect->SetTexture( hDiffuseMap, mesh.diffuseMap ) );

				D3DXHANDLE hNormalMap = mesh.effect->GetParameterBySemantic( NULL, "NORMAL_MAP" );
				graphics.ErrorCheck( mesh.effect->SetTexture( hNormalMap, mesh.normalMap ) );

				D3DXHANDLE hLightDirection = mesh.effect->GetParameterBySemantic( NULL, "LIGHTDIRECTION" );
				D3DXVECTOR4 lightDirection = Math::Vector( 0.5, 1, 0 ).Normal();
				graphics.ErrorCheck( mesh.effect->SetVector( hLightDirection, &lightDirection ) );

				// store handles for dynamic shader parameters
				mesh.EffectParameters.viewProjection = mesh.effect->GetParameterBySemantic( NULL, "VIEWPROJECTION" );
				mesh.EffectParameters.boneTransforms = mesh.effect->GetParameterBySemantic( NULL, "BONE_TRANSFORMS" );
				mesh.EffectParameters.shaderTest = mesh.effect->GetParameterBySemantic( NULL, "SHADER_TEST" );

				for( int t=0; t<SAM_COUNT; ++t )
				{
					mesh.EffectParameters.mTechniques[t] = mesh.effect->GetTechniqueByName( mSkeletons[t]->GetShaderTechnique() );
				}
			}
		}
		//*/
	}
}

void MeshManager::ReleaseResources( void )
{
	for( unsigned int m = 0; m < meshComponents.size(); ++m )
	{
		log.Message( "Releasing Resources" );

		Mesh& mesh = meshComponents[m];

		if( mesh.effect )
		{
			mesh.effect->Release();
			mesh.effect = NULL;
		}

		if( mesh.indexBuffer )
		{
			mesh.indexBuffer->Release();
			mesh.indexBuffer = NULL;
		}

		if( mesh.vertexBuffer )
		{
			mesh.vertexBuffer->Release();
			mesh.vertexBuffer = NULL;
		}

		if( mesh.vertexDeclaration )
		{
			mesh.vertexDeclaration->Release();
			mesh.vertexDeclaration = NULL;
		}
	}
}

void MeshManager::Update( void )
{
}

void MeshManager::Render( void )
{
	graphics.Device()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	graphics.Device()->SetRenderState( D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	graphics.Device()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE);
	graphics.Device()->SetRenderState( D3DRS_SRGBWRITEENABLE, TRUE );

	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_LIGHTING, false ), "Setting Lighting State Failed" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW ), "Setting Culling Mode Failed" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE ), "Enabling Depth Testing" );
	graphics.ErrorCheck( graphics.Device()->SetRenderState( D3DRS_COLORVERTEX, TRUE ), "Enabling Vertex Colours" );

	for( unsigned int m = 0; m < meshComponents.size(); ++m )
	{
		log.Message( "Rendering mesh" );
		
		const Mesh& mesh = meshComponents[m];

		//if( mesh.effect )
		//{
			/*/ set dynamic shader parameters
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
		//}
	}
	
	graphics.Device()->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	graphics.ErrorCheck( graphics.Device()->SetStreamSource( 0, NULL, 0, 0 ), "Clearing stream source" );
	graphics.ErrorCheck( graphics.Device()->SetIndices( NULL ), "Clearing indices" );
}