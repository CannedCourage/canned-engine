#ifndef _ANIMATEDSPRITECOMPONENT_H_
#define _ANIMATEDSPRITECOMPONENT_H_

#include <vector>
#include <chrono>
#include "Engine/Time.h"
#include "Sprite/SpriteComponent.h"

class Frame : public SpriteComponent
{
private:
protected:
public:

	EngineDuration Interval;

	Frame( const int _Duration = 0 );
	Frame( const EngineDuration& _Duration = EngineDuration::zero() );
};
 
//TODO: This needs callbacks for animation events (begin, end, custom etc.)
class Animation2D
{
private:
protected:

	unsigned int CurrentFrame = 0;

	EngineDuration ElapsedTime = EngineDuration::zero();

	short int Direction = 1;

	void NextFrame( void );
public:

	enum class PlaybackMode : unsigned int { ONCE = 10, LOOP = 20, OSCILLATE = 30 };

	std::vector<Frame> Frames;

	std::string Name;
	
	float Speed = 1.0f; //frames per second? Multiplier on time delta?

	PlaybackMode Mode = Animation2D::PlaybackMode::ONCE;

	bool Playing = false;

	Animation2D( const std::string& _Name );

	void Play( PlaybackMode _Mode = PlaybackMode::ONCE );
	void Update( const EngineDuration& DeltaT );
	void Rewind( void );

	void AddFrame( const Frame& NewFrame );

	Frame& GetCurrentFrame( void );
	const Frame& GetCurrentFrame( void ) const;

	EngineDuration Length( void ) const;
};
 
class AnimatedSpriteComponent
{
private:
protected:
public:

	std::vector<Animation2D> Animations;
	
	int CurrentAnimation = -1;
	int NextAnimation = -1;

	AnimatedSpriteComponent( void );

	void Update( const EngineDuration& DeltaT );

	void PlayAnimation( const std::string& Name, Animation2D::PlaybackMode Mode );
	void QueueAnimation( const std::string& Name, Animation2D::PlaybackMode Mode );

	void AddAnimation( Animation2D& Anim );

	Animation2D& GetCurrentAnimation( void );
	const Animation2D& GetCurrentAnimation( void ) const;
};

#endif //_ANIMATEDSPRITECOMPONENT_H_