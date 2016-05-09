#include "Engine/ProcessManager.h"
#include "Engine/iProcess.h"

void ProcessManager::AddProcess( iProcess* Process, Priority ProcessPriority )
{
	Processes.insert( std::pair<iProcess*, Priority>( Process, ProcessPriority ) );

	Process->Start();
}

//TODO: Should this destruct the Process object?
void ProcessManager::RemoveProcess( iProcess* Process )
{
	Process->End();

	Processes.erase( Process );
}

void ProcessManager::UpdateProcesses( const EngineDuration& DeltaT, Priority ProcessPriority )
{
	for( auto&& itr : Processes )
	{
		if( itr.second == ProcessPriority )
		{
			if( itr.first != NULL )
			{
				itr.first->Update( DeltaT );
			}
		}
	}
}