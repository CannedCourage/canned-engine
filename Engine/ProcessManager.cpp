#include "Engine/ProcessManager.h"
#include "Engine/iProcess.h"

ProcessManager::ProcessManager( void ) : processes( ), mapIt( )
{
}

ProcessManager::~ProcessManager( void )
{
}

void ProcessManager::AddProcess( iProcess* process, Priority priority )
{
	processes.insert( std::pair<iProcess*, Priority>( process, priority ) );
	process->Start();
}

void ProcessManager::RemoveProcess( iProcess* process )
{
	process->End();
	processes.erase( process );
}

void ProcessManager::UpdateProcesses( const double& deltaT, Priority priority )
{
	for( mapIt = processes.begin(); mapIt != processes.end(); mapIt++ )
	{
		if( mapIt->second == priority )
		{
			if( mapIt->first ){ mapIt->first->Update( deltaT ); }
		}
	}
}