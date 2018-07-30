#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "GUI/GUI.h"

class MainMenu : public GUI
{
private:
protected:
public:
 
    MainMenu( const char* _name, GUI* _parent = NULL );
    ~MainMenu( void );

    void Init( void );
    void Update( void );
    void Render( void );
    void CleanUp( void );

    void WidgetSelected( Widget* caller );
    void WidgetFocused( Widget* caller );
};

#endif //_MAINMENU_H_