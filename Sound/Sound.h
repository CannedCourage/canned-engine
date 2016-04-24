//Sound service
//Initialise FMOD

#ifndef _SOUND_H_
#define _SOUND_H_

#pragma comment(lib, "fmodL64_vc.lib")
#pragma comment(lib, "fmodstudioL64_vc.lib")

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "fmod_studio_common.h"

#include "Logging/Log.h"

#include <string>
#include <unordered_map>

class Sound
{
private:
protected:

	Log log;

	FMOD::Studio::System* studio = NULL;
	FMOD::System* lowlevel = NULL;
	unsigned int version = 0;

	std::unordered_map<std::string, FMOD::Sound*> SoundAssets;
	
	void ReleaseSounds( void );
public:

	Sound( void );
	~Sound( void );

	FMOD::Studio::System* Studio( void );
	FMOD::System* System( void );

	void Init( void );
	void Update( void );
	void CleanUp( void );

	bool LoadSoundSample( const std::string& File, const std::string& Name );
	bool LoadSoundStream( const std::string& File, const std::string& Name );
	FMOD::Sound* GetSound( const std::string& Name );

	void ErrorCheck( FMOD_RESULT result, const std::string& Info );
};

#endif //_SOUND_H_