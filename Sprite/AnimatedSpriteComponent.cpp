#include "Sprite/AnimatedSpriteComponent.h"
#include <algorithm>
#include <numeric>

//FRAME//
Frame::Frame( const int _Duration ) : SpriteComponent(), Interval( _Duration )
{
}

Frame::Frame( const EngineDuration& _Duration ) : Interval( _Duration )
{
}

//ANIMATION2D//
Animation2D::Animation2D( const std::string& _Name ) : Frames(), CurrentFrame( 0 ), ElapsedTime( EngineDuration::zero() ), Speed( 1.0f ), Mode( Animation2D::PlaybackMode::ONCE ), Name( _Name ), Playing( false ), Direction( 1 )
{
}

void Animation2D::Play( PlaybackMode _Mode )
{
	Mode = _Mode;
	Playing = true;
}

void Animation2D::Update( const EngineDuration& DeltaT )
{
	if( !Playing )
	{
		return;
	}

	//EngineDuration displayDelta( std::chrono::duration_cast<EngineDuration>( DeltaT ) );
	EngineDuration displayDelta = DeltaT;

	const Frame& frame = Frames[CurrentFrame];

	if( ( ElapsedTime + displayDelta ) > frame.Interval )
	{
		auto frameTimeLeft = frame.Interval - ElapsedTime; //Calculate how much time left before the frame changes
		auto remainingTime = displayDelta - frameTimeLeft; //Add the extra time to the timer after the frame change

		NextFrame();

		ElapsedTime = remainingTime;
	}
	else
	{
		ElapsedTime += displayDelta;
	}
}

void Animation2D::Rewind( void )
{
	ElapsedTime = EngineDuration::zero();
	CurrentFrame = 0;
}

void Animation2D::NextFrame( void )
{
	switch( Mode )
	{
	case PlaybackMode::ONCE:
		if( Direction > 0 )
		{
			if( CurrentFrame < ( Frames.size() - 1 ) )
			{
				CurrentFrame++;
			}
			else
			{
				Playing = false;
			}
		}
		else
		{
			if( CurrentFrame > 0 )
			{
				CurrentFrame--;
			}
			else
			{
				Playing = false;
			}
		}
		return;
	case PlaybackMode::LOOP:
		CurrentFrame += Direction;

		if( Direction > 0 )
		{
			if( CurrentFrame >= ( Frames.size() ) )
			{
				CurrentFrame = 0;
			}
		}
		else
		{
			if( CurrentFrame == 0 )
			{
				CurrentFrame = ( Frames.size() - 1 );
			}
		}

		return;
	case PlaybackMode::OSCILLATE:
		if( Direction > 0 )
		{
			if( CurrentFrame < ( Frames.size() - 1 ) )
			{
				CurrentFrame++;
			}
		}
		else
		{
			if( CurrentFrame > 0 )
			{
				CurrentFrame--;
			}
		}

		if( ( CurrentFrame == 0 ) || ( CurrentFrame == ( Frames.size() - 1 ) ) )
		{
			Direction *= -1;
		}
		return;
	default:
		return;
	}
}

void Animation2D::AddFrame( const Frame& NewFrame )
{
	Frames.push_back( NewFrame );
}

Frame& Animation2D::GetCurrentFrame( void )
{
	return Frames.at( CurrentFrame );
}

const Frame& Animation2D::GetCurrentFrame( void ) const
{
	return Frames.at( CurrentFrame );
}

//Interesting note: This method returns a reference to a temporary object
//The result of this method can only be stored in a const reference, it will fail at compilation if non-const
//The liftime of the temporary becomes the lifetime of the const reference it's stored in
const EngineDuration& Animation2D::Length( void ) const
{
	return std::accumulate( Frames.begin(), Frames.end(), EngineDuration::zero(),
							[]( const EngineDuration& a, const Frame& b )
							{
								return a + b.Interval;
							}
							);
}

//ANIMATEDSPRITECOMPONENT//
AnimatedSpriteComponent::AnimatedSpriteComponent( void ) : Animations(), CurrentAnimation( -1 ), NextAnimation( -1 )
{
}

void AnimatedSpriteComponent::Update( const EngineDuration& DeltaT )
{
	//If current animation has finished, move to next queued animation (if valid)
	if( ( Animations[CurrentAnimation].Playing == false ) && ( NextAnimation >= 0 ) )
	{
		CurrentAnimation = NextAnimation;
		NextAnimation = -1;
	}

	//Update current animation
	if( ( CurrentAnimation >= 0 ) && ( CurrentAnimation < Animations.size() ) )
	{
		Animations[CurrentAnimation].Update( DeltaT );
	}
}

void AnimatedSpriteComponent::PlayAnimation( const std::string& Name, Animation2D::PlaybackMode Mode )
{
	auto result = std::find_if( Animations.begin(), Animations.end(), 	[&Name]( const Animation2D& Anim )
																		{
																			return Anim.Name == Name;
																		}
																		);

	CurrentAnimation = ( result - Animations.begin() ); //Deduce index of animation - this is potentially expensive

	Animations[CurrentAnimation].Rewind();
	Animations[CurrentAnimation].Play( Mode );
}

void AnimatedSpriteComponent::QueueAnimation( const std::string& Name, Animation2D::PlaybackMode Mode )
{
	auto result = std::find_if( Animations.begin(), Animations.end(), 	[&Name]( const Animation2D& Anim )
																		{
																			return Anim.Name == Name;
																		}
																		);

	NextAnimation = ( result - Animations.begin() ); //Deduce index of animation - this is potentially expensive
}

void AnimatedSpriteComponent::AddAnimation( Animation2D& Anim )
{
	Animations.push_back( Anim );
}

Animation2D& AnimatedSpriteComponent::GetCurrentAnimation( void )
{
	return Animations[CurrentAnimation];
}

const Animation2D& AnimatedSpriteComponent::GetCurrentAnimation( void ) const
{
	return Animations[CurrentAnimation];
}