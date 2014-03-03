#include "Mesh/MeshManager.h"

MeshManager::MeshManager( void ) : log( "MeshManager" )
{
}

MeshManager::~MeshManager( void )
{
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

	return true;
}