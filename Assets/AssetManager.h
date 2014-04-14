#ifndef _ASSETMANAGER_H_
#define _ASSETMANAGER_H_

//Assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <assimp/Importer.hpp>

#include <vector>
#include "Mesh/Mesh.h"

#include "fmod.hpp"

#include "Logging\Log.h"

class System;
class Graphics;
class Sound;

class AssetManager
{
private:
protected:

	Graphics& graphics;
	Sound& sound;

	Log log;

	//Meshes

	std::vector<Mesh> meshAssets;

	void GetVertexInformation( const aiMesh* mesh, MeshData* data );
	void BuildMesh( const aiScene* sc );

	void AccquireMeshResources( Mesh& mesh );
	void ReleaseMeshResources( Mesh& mesh );

	//Sounds
	
	void ReleaseSounds( void );
	
	//Replace with sound class wrapper
	std::vector<FMOD::Sound*> soundAssets;
public:

	AssetManager( System& sys );
	~AssetManager( void );

	void OnLost( void );
	void OnRecover( void );

	//Meshes

	bool LoadMesh( const char* file );
	const Mesh& GetMesh( const unsigned int meshID );

	//Sounds
	
	bool LoadSoundSample( const char* file );
	bool LoadSoundStream( const char* file );
	FMOD::Sound* GetSound( const unsigned int meshID );

};

#endif //_ASSETMANAGER_H_