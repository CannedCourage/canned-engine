#ifndef _ASSETMANAGER_H_
#define _ASSETMANAGER_H_

#pragma comment(lib, "assimp.lib")

//Assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <assimp/Importer.hpp>

#include <map>
#include <string>

#include "Logging\Log.h"
#include "Mesh/Mesh.h"

class System;

class AssetManager
{
private:
protected:

	Log log;

	//Meshes

	std::map<std::string, Mesh> meshAssets;

	void GetVertexInformation( const aiMesh* mesh, MeshData* data );
	void BuildMesh( const aiScene* sc, const char* name );

	void AccquireMeshResources( Mesh& mesh );
	void AccquireAllMeshes( void );
	void ReleaseMeshResources( Mesh& mesh );
	void ReleaseAllMeshes( void );
public:

	AssetManager( System& sys );
	~AssetManager( void );

	void OnLost( void );
	void OnRecover( void );

	void CleanUp( void );

	//Meshes

	bool LoadMesh( const char* file );
	const Mesh& GetMesh( const char* name );
};

#endif //_ASSETMANAGER_H_