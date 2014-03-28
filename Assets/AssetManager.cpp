#include "Assets/AssetManager.h"

#include "System/System.h"
#include "Graphics/Graphics.h"

AssetManager::AssetManager( System& sys ) : log( "AssetManager" ), graphics( sys.graphics )
{
}

AssetManager::~AssetManager( void )
{
	for( unsigned int m = 0; m < meshAssets.size(); ++m )
	{
		ReleaseMeshResources( meshAssets[m] );
	}
}