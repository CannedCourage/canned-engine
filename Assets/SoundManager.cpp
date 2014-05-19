#include "Assets/AssetManager.h"
#include "Sound/Sound.h"

bool AssetManager::LoadSoundSample( const char* file )
{
	std::string name( file );

	FMOD::Sound* newSound = NULL;
	sound.ErrorCheck( sound.System()->createSound( file, FMOD_CREATESAMPLE, NULL, &newSound ), "Loading Uncompressed Sound" );
	soundAssets[ name ] = newSound;

	return true;
}

bool AssetManager::LoadSoundStream( const char* file )
{
	std::string name( file );

	FMOD::Sound* newSound = NULL;
	sound.ErrorCheck( sound.System()->createSound( file, FMOD_CREATESTREAM, NULL, &newSound ), "Loading Compressed Sound" );
	soundAssets[ name ] = newSound;

	return true;
}

void AssetManager::ReleaseSounds( void )
{
	std::map<std::string, FMOD::Sound*>::iterator it;

	for(it = soundAssets.begin(); it != soundAssets.end(); it++)
	{
		if( it->second )
		{
			sound.ErrorCheck( it->second->release(), "Releasing Sounds" );

			it->second = NULL; //Always null your pointers
		}
	}
}

FMOD::Sound* AssetManager::GetSound( const char* name )
{
	std::string str( name );

	return soundAssets[ name ];
}