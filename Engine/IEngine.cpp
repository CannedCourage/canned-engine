#include "Engine\IEngine.h"
#include "System\System.h"
//#include "Engine\Modules.h"

IEngine::IEngine( System &s ) : system( s ),
								manager( s.sceneManager ),
								settings( s.settings ),
								graphics( s.graphics )
								//input( s.input ),
								//sound( s.sound )
{
}

IEngine::~IEngine( void )
{
}

void IEngine::OnLost( void )
{
	//Device has been lost, destroy resources
}

void IEngine::OnRecover( void )
{
	//Device has recovered, recreate resources
}