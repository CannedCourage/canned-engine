#include "Assets/AssetManager.h"
#include "Sound/Sound.h"

bool AssetManager::LoadSoundSample( const char* file )
{
	FMOD::Sound* newSound = NULL;
	sound.ErrorCheck( sound.System()->createSound( file, FMOD_CREATESAMPLE, NULL, &newSound ), "Loading Uncompressed Sound" );
	soundAssets.push_back( newSound );

	return true;
}

bool AssetManager::LoadSoundStream( const char* file )
{
	FMOD::Sound* newSound = NULL;
	sound.ErrorCheck( sound.System()->createSound( file, FMOD_CREATESTREAM, NULL, &newSound ), "Loading Compressed Sound" );
	soundAssets.push_back( newSound );

	return true;
}

void AssetManager::ReleaseSounds( void )
{
	for( unsigned int s = 0; s < soundAssets.size(); s++ )
	{
		//Still erroring :(
		if( soundAssets[s] )
		{
			sound.ErrorCheck( soundAssets[s]->release(), "Releasing Sounds" );
			soundAssets[s] = NULL; //Always null your pointers
		}
	}
}

FMOD::Sound* AssetManager::GetSound( const unsigned int soundID )
{
	return soundAssets[soundID];
}