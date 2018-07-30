#ifndef _GUI_H_
#define _GUI_H_

#include <vector>

class Widget;

class GUI
{
private:
protected:

    const char* name;

    std::vector<Widget*> controls;

    GUI* parent;
public:

    GUI( const char* _name, GUI* _parent = NULL );
    virtual ~GUI( void );

    virtual void Init( void ) = 0;
    virtual void Update( void ) = 0;
    virtual void Render( void ) = 0;
    virtual void CleanUp( void ) = 0;

    virtual void WidgetSelected( Widget* caller ) = 0;
    virtual void WidgetFocused( Widget* caller ) = 0;
};

#endif //_GUI_H_