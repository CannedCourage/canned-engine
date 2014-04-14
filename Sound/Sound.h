//Sound service
//Initialise FMOD

#ifndef _SOUND_H_
#define _SOUND_H_

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "fmod_studio_common.h"

class Sound
{
private:
protected:

	FMOD::Studio::System* studio;
	FMOD::System* lowlevel;
	FMOD::Channel* channel;
	unsigned int version;
public:

	Sound( void );
	~Sound( void );

	FMOD::Studio::System* Studio( void );
	FMOD::System* System( void );

	void Init( void );
	void Update( void );
	void CleanUp( void );

	void ErrorCheck( FMOD_RESULT result, const char* const info );
};

#endif //_SOUND_H_