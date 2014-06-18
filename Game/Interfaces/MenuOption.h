#ifndef _MENUOPTION_H_
#define _MENUOPTION_H_

#include "GUI/Widget.h"
#include "Graphics/Graphics.h"

class System;
class GUI;

class MenuOption : public Widget
{
protected:

    int index;
    MenuOption* prev;
    MenuOption* next;

    const char* displayText;

    ID3DXSprite* sprite;
    LPD3DXFONT font;
    RECT FontPosition;
public:

    MenuOption( System& sys, const char* _name, GUI& _parent );
    ~MenuOption( void );

    void SetDisplayText( const char* text );
    void SetPosition( int x, int y );
    void SetNext( MenuOption* _next );
    void SetPrev( MenuOption* _prev );

    void NextOption( void );
    void PreviousOption( void );

    void Update( void );
    void Render( void );
};

#endif //_MENUOPTION_H_