#ifndef _WIDGET_H_
#define _WIDGET_H_

class System;
class Graphics;
class GUI;
 
class Widget
{
private:
protected:

    const char* name;

    int x, y;
    bool active, focused;

    GUI& parent;

    Graphics& graphics;
public:

    Widget( System& sys, const char* _name, GUI& _parent );
    virtual ~Widget( void );

    virtual bool hasFocus( void );
    virtual void Focus( void );
    virtual void Blur( void );

    virtual void Update( void ) = 0;
    virtual void Render( void ) = 0;
};

#endif //_WIDGET_H_