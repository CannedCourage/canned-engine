#ifndef _MESH_MANAGER_H_
#define _MESH_MANAGER_H_

//Assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <assimp/Importer.hpp>

#include "Logging\Log.h"

class MeshManager
{
private:
protected:

	Log log;
public:

	MeshManager( void );
	~MeshManager( void );

	bool LoadMesh( const char* file );
};

#endif //_MESH_MANAGER_H_