//Base GUI class

#ifndef _GUI_H_
#define _GUI_H_

#include "SceneManager\IScene.h"
#include "GUI\Widget.h"
#include <list>

class GUI : public IScene
{
private:
protected:

	std::list<Widget*> widgets;
	std::list<Widget*>::iterator itr;

	void AddWidget( Widget* w );
public:

	GUI( const char* n = "DEFAULT_GUI" );
	~GUI( void );

	virtual void WidgetClicked(Widget* widget) = 0;
};

#endif //_GUI_H_