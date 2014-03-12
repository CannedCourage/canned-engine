#ifndef _MESH_MANAGER_H_
#define _MESH_MANAGER_H_

//Assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <assimp/Importer.hpp>

#include "Logging\Log.h"

#include "Mesh\Mesh.h"

#include <vector>

class Graphics;

class MeshManager
{
private:
protected:

	Graphics& graphics;

	std::vector<Mesh> meshComponents; //Change to map, with entity id as key?

	Log log;

	void GetVertexInformation( const aiMesh* mesh, MeshData* data );
	void BuildMesh( const aiScene* sc );
	void AccquireResources( void );
	void ReleaseResources( void );
public:

	MeshManager( Graphics& g );
	~MeshManager( void );

	bool LoadMesh( const char* file );

	void Update( void );
	void Render( void );
};

#endif //_MESH_MANAGER_H_