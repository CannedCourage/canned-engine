#ifndef _PROCESSMANAGER_H_
#define _PROCESSMANAGER_H_

#include <map>
#include "Engine/Time.h"

class iProcess;

enum Priority{ PRE_UPDATE = 10, UPDATE = 20, FIXED_UPDATE = 30, POST_UPDATE = 40, PRE_RENDER = 50, RENDER = 60, POST_RENDER = 70 };

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