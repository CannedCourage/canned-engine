#ifndef _ASSETMANAGER_H_
#define _ASSETMANAGER_H_

//Assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <assimp/Importer.hpp>

#include <vector>
#include "Mesh/Mesh.h"

#include "Logging\Log.h"

class Graphics;

class AssetManager
{
private:
protected:

	Graphics& graphics;

	Log log;

	std::vector<Mesh> meshAssets;

	void GetVertexInformation( const aiMesh* mesh, MeshData* data );
	void BuildMesh( const aiScene* sc );

	void AccquireMeshResources( void );
	void ReleaseMeshResources( void );
public:

	AssetManager( Graphics& g );
	~AssetManager( void );

	bool LoadMesh( const char* file );
};

#endif //_ASSETMANAGER_H_