#include "Sound/Sound.h"
#include <exception>

Sound::Sound( void ) : log( "Sound" )
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
    TRACE( "Sound Initialisation" );
    
	ErrorCheck( FMOD::Studio::System::create( &studio ), "Creating System" );

    ErrorCheck( studio->getLowLevelSystem( &lowlevel ), "Getting Low Level System" );

    ErrorCheck( lowlevel->getVersion( &version ), "Getting version" );

    if( version < FMOD_VERSION )
    {
        std::string message = "FMOD lib version ";
        message += version + " doesn't match header version " + FMOD_VERSION;

        throw std::exception( message.c_str() );
    }

    //Args: maxChannels, Studio flags, lowlevel flags, extraDriverData
    ErrorCheck( studio->initialize( 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL ), "System Init" );
}

void Sound::Update( void )
{
    if( studio != NULL )
    {
        studio->update();
    }

    if( lowlevel != NULL )
    {
        lowlevel->update();
    }
}

void Sound::CleanUp( void )
{
    if( studio != NULL )
    {
        studio->release();
    }

    if( lowlevel )
    {
        lowlevel->release();
    }
}

bool Sound::LoadSoundSample( const std::string& File, const std::string& Name )
{
    FMOD::Sound* newSound = NULL;

    ErrorCheck( lowlevel->createSound( File.c_str(), FMOD_CREATESAMPLE, NULL, &newSound ), "Loading Uncompressed Sound" );
    
    SoundAssets[Name] = newSound;

    return true;
}

bool Sound::LoadSoundStream( const std::string& File, const std::string& Name )
{
    FMOD::Sound* newSound = NULL;

    ErrorCheck( lowlevel->createSound( File.c_str(), FMOD_CREATESTREAM, NULL, &newSound ), "Loading Compressed Sound" );
    
    SoundAssets[Name] = newSound;

    return true;
}

FMOD::Sound* Sound::GetSound( const std::string& Name )
{
    return SoundAssets.at( Name );
}

void Sound::ReleaseSounds( void )
{
    std::unordered_map<std::string, FMOD::Sound*>::iterator it;

    for( it = SoundAssets.begin(); it != SoundAssets.end(); it++ )
    {
        if( it->second )
        {
            ErrorCheck( it->second->release(), "Releasing Sounds" );

            it->second = NULL; //Always null your pointers
        }
    }
}

void Sound::ErrorCheck( FMOD_RESULT result, const std::string& Info )
{
	if( result != FMOD_OK )
    {
        std::string message = "FMOD error ";
        message += result + " - ";
        message += FMOD_ErrorString( result );
        message += ": " + Info;

        TRACE( message );

        throw std::exception( message.c_str() );
    }
}