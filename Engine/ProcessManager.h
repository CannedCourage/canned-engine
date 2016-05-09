#ifndef _PROCESSMANAGER_H_
#define _PROCESSMANAGER_H_

#include <unordered_map>
#include "Engine/Time.h"

class iProcess;

enum class Priority : unsigned int
{
	PRE_UPDATE,
	UPDATE,
	FIXED_UPDATE,
	POST_UPDATE,
	PRE_RENDER,
	RENDER,
	POST_RENDER
};

typedef std::unordered_map<iProcess*, Priority> ProcessMap;

class ProcessManager
{
private:
protected:

	 ProcessMap Processes{};
public:

	ProcessManager( void ) = default;
	~ProcessManager( void ) = default;

	void AddProcess( iProcess* Process, Priority ProcessPriority );
	void RemoveProcess( iProcess* Process );

	void UpdateProcesses( const EngineDuration& DeltaT, Priority ProcessPriority );
};

#endif //_PROCESSMANAGER_H_