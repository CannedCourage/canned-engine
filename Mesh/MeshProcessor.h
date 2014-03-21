#ifndef _MESHPROCESSOR_H_
#define _MESHPROCESSOR_H_

#include "Mesh/MeshComponent.h"

#include "Logging/Log.h"

#include <map>

class Graphics;

class MeshProcessor
{
private:
protected:

	Graphics& graphics;

	std::map<int, MeshComponent> meshComponents; //entity id as key

	Log log;
public:

	MeshProcessor( Graphics& g );
	~MeshProcessor( void );

	MeshComponent& GetMeshComponent( int entityID );

	void Update( const float& dT );
	void Render( const float& dT );
};

#endif //_MESHPROCESSOR_H_