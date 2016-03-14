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

	unsigned int CurrentFrame;

	EngineDuration ElapsedTime;

	short int Direction;

	void NextFrame( void );
public:

	enum class PlaybackMode : unsigned int { ONCE = 10, LOOP = 20, OSCILLATE = 30 };

	std::vector<Frame> Frames;

	std::string Name;
	
	float Speed; //frames per second? Multiplier on time delta?

	PlaybackMode Mode;

	bool Playing;

	Animation2D( const std::string& _Name );

	void Play( PlaybackMode _Mode = PlaybackMode::ONCE );
	void Update( const EngineDuration& DeltaT );
	void Rewind( void );

	void AddFrame( const Frame& NewFrame );

	Frame& GetCurrentFrame( void );
	const Frame& GetCurrentFrame( void ) const;

	const EngineDuration& Length( void ) const;
};
 
class AnimatedSpriteComponent
{
private:
protected:
public:

	std::vector<Animation2D> Animations;
	
	int CurrentAnimation;
	int NextAnimation;

	AnimatedSpriteComponent( void );

	void Update( const EngineDuration& DeltaT );

	void PlayAnimation( const std::string& Name, Animation2D::PlaybackMode Mode );
	void QueueAnimation( const std::string& Name, Animation2D::PlaybackMode Mode );

	void AddAnimation( Animation2D& Anim );

	Animation2D& GetCurrentAnimation( void );
	const Animation2D& GetCurrentAnimation( void ) const;
};

#endif //_ANIMATEDSPRITECOMPONENT_H_