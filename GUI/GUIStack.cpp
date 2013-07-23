#include "SceneManager\SceneManager.h"
#include "GUI\GUI.h"

void SceneManager::PushGUI( GUI* const newGUI )
{
	GUIStack.push_front( newGUI );
	GUIStack.front()->Load();
}

GUI* const SceneManager::PopGUI( void )
{
	GUIStack.front()->Unload();
	GUI* temp = GUIStack.front();
	GUIStack.pop_front();
	return temp;
}

GUI* const SceneManager::SwapToGUI( GUI* const newGUI )
{
	GUI* temp = PopGUI();
	PushGUI( newGUI );
	return temp;
}

void SceneManager::ClearGUIStack( void )
{
	while( !GUIStack.empty() )
	{
		GUIStack.front()->Unload();
		delete GUIStack.front();
		GUIStack.pop_front();
	}
}