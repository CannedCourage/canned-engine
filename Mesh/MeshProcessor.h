#ifndef _MESHPROCESSOR_H_
#define _MESHPROCESSOR_H_

#include "Engine/iProcess.h"

#include "Mesh/MeshComponent.h"

#include "Logging/Log.h"

#include <map>

class Graphics;

class MeshProcessor : public iProcess
{
private:
protected:

	Graphics& graphics;

	std::map<int, MeshComponent> meshComponents; //entity id as key

	Log log;
public:

	MeshProcessor( Graphics& g );
	~MeshProcessor( void );

	void AddMeshComponent( const unsigned int entityID, const Mesh& meshAsset );
	MeshComponent& GetMeshComponent( const unsigned int entityID );

	void Start( void );
	void Update( const EngineDuration& deltaT );
	void End( void );
};

#endif //_MESHPROCESSOR_H_