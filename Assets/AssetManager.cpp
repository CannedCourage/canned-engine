#include "Assets/AssetManager.h"

#include "System/System.h"
#include "Graphics/Graphics.h"
#include "Sound/Sound.h"

AssetManager::AssetManager( System& sys ) : log( "AssetManager" ), graphics( sys.graphics ), sound( sys.sound )
{
}

AssetManager::~AssetManager( void )
{
	CleanUp();
}

void AssetManager::CleanUp( void )
{
	for( unsigned int m = 0; m < meshAssets.size(); m++ )
	{
		ReleaseMeshResources( meshAssets[m] );
	}

	ReleaseSounds();
}

void AssetManager::OnLost( void )
{
	for( unsigned int m = 0; m < meshAssets.size(); m++ )
	{
		ReleaseMeshResources( meshAssets[m] );
	}
}

void AssetManager::OnRecover( void )
{
	for( unsigned int m = 0; m < meshAssets.size(); m++ )
	{
		AccquireMeshResources( meshAssets[m] );
	}
}