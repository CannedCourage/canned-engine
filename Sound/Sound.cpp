#include "Sound/Sound.h"

#include <sstream>

Sound::Sound( void ) : studio( NULL ), lowlevel( NULL ), version( NULL ), channel( NULL )
{
}

Sound::~Sound( void )
{
}

FMOD::Studio::System* Sound::Studio( void )
{
    return studio;
}

FMOD::System* Sound::System( void )
{
    return lowlevel;
}

void Sound::Init( void )
{
	ErrorCheck( FMOD::Studio::System::create( &studio ), "Creating System" );

    ErrorCheck( studio->getLowLevelSystem( &lowlevel ), "Getting Low Level System" );

    ErrorCheck( lowlevel->getVersion( &version ), "Getting version" );

    if( version < FMOD_VERSION )
    {
        std::ostringstream o;

        o << "FMOD lib version " << version << " doesn't match header version " << FMOD_VERSION;

        throw( o.str().c_str() );
    }

    //Args: maxChannels, Studio flags, lowlevel flags, extraDriverData
    ErrorCheck( studio->initialize( 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL ), "System Init" );
}

void Sound::Update( void )
{
    studio->update();
}

void Sound::CleanUp( void )
{
    studio->release();
}

void Sound::ErrorCheck( FMOD_RESULT result, const char* const info )
{
	if( result != FMOD_OK )
    {
        std::ostringstream o;

        o << "FMOD error " << result << " - " << FMOD_ErrorString( result ) << ": " << info;

        throw( o.str().c_str() );
    }
}