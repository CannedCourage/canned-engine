#ifndef _ASSETMANAGER_H_
#define _ASSETMANAGER_H_

//Assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <assimp/Importer.hpp>

#include <map>
#include <string>

#include "Mesh/Mesh.h"

#include "fmod.hpp"

#include "Logging\Log.h"

#include <d3dx9.h>

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

	std::map<std::string, Mesh> meshAssets;

	void GetVertexInformation( const aiMesh* mesh, MeshData* data );
	void BuildMesh( const aiScene* sc, const char* name );

	void AccquireMeshResources( Mesh& mesh );
	void AccquireAllMeshes( void );
	void ReleaseMeshResources( Mesh& mesh );
	void ReleaseAllMeshes( void );

	//Sounds
	
	void ReleaseSounds( void );
	
	std::map<std::string, FMOD::Sound*> soundAssets;

	//Textures

	std::map<std::string, IDirect3DTexture9*> textureAssets;

	void ReleaseTextures( void );
public:

	AssetManager( System& sys );
	~AssetManager( void );

	void OnLost( void );
	void OnRecover( void );

	void CleanUp( void );

	//Meshes

	bool LoadMesh( const char* file );
	const Mesh& GetMesh( const char* name );

	//Sounds
	
	bool LoadSoundSample( const char* file );
	bool LoadSoundStream( const char* file );
	FMOD::Sound* GetSound( const char* name );

	//Textures
	
	void LoadTexture( const char* filename );
	IDirect3DTexture9* GetTexture( const char* filename );
};

#endif //_ASSETMANAGER_H_