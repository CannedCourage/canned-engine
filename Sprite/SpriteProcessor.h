#ifndef _SPRITEPROCESSOR_H_
#define _SPRITEPROCESSOR_H_

#include "Engine/iProcess.h"

#include "Sprite/SpriteComponent.h"

#include "Logging/Log.h"

#include <map>

class Graphics;

class SpriteProcessor : public iProcess
{
private:
protected:

	Graphics& graphics;

	ID3DXSprite* spriteInterface;

	std::map<int, SpriteComponent> spriteComponents; //entity id as key

	Log log;
public:

	SpriteProcessor( Graphics& g );
	~SpriteProcessor( void );

	void AddSpriteComponent( const unsigned int entityID, IDirect3DTexture9* _texture );
	SpriteComponent& GetSpriteComponent( const unsigned int entityID );

	void Start( void );
	void Update( const double& deltaT );
	void End( void );
};

#endif //_SPRITEPROCESSOR_H_