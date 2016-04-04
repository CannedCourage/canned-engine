#ifndef _PROCESSMANAGER_H_
#define _PROCESSMANAGER_H_

#include <map>
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

class ProcessManager
{
private:
protected:

	std::map<iProcess*, Priority> processes;
	std::map<iProcess*, Priority>::iterator mapIt;
public:

	ProcessManager( void );
	~ProcessManager( void );

	void AddProcess( iProcess* process, Priority priority );
	void RemoveProcess( iProcess* process );

	void UpdateProcesses( const EngineDuration& deltaT, Priority priority );
};

#endif //_PROCESSMANAGER_H_