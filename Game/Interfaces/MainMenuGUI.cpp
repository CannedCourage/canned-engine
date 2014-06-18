#include "Game/Interfaces/MainMenuGUI.h"
#include "Game/Interfaces/MenuOption.h"

MainMenu::MainMenu( System& sys, const char* _name, GUI* _parent ) : GUI( sys, _name, _parent )
{
    MenuOption* one = new MenuOption( sys, "Option 1", *this );
    one->SetDisplayText( "This is Option 1" );
    one->SetPosition( 100, 100 );

	controls.push_back( one );
}

MainMenu::~MainMenu( void )
{
}

void MainMenu::Init( void )
{
}

void MainMenu::Update( void )
{
	std::vector<Widget*>::iterator it;

    for( it = controls.begin(); it != controls.end(); it++ )
    {
        if( ( *it ) )
        {
            ( *it )->Update();
        }
    }

    //Move cursor
}

void MainMenu::Render( void )
{
	std::vector<Widget*>::iterator it;

    for( it = controls.begin(); it != controls.end(); it++ )
    {
        if( ( *it ) )
        {
            ( *it )->Render();
        }
    }
}

void MainMenu::CleanUp( void )
{
}

void MainMenu::WidgetSelected( Widget* caller )
{
    //Do logic based on widget name
    //if( strcmp( caller.name, "Option 1" ) )
    //{
        //Do Stuff such as change game scene
    //}
}

void MainMenu::WidgetFocused( Widget* caller )
{
    //Do logic based on widget name
    //if( strcmp( caller.name, "Option 1" ) )
    //{
        //Do Stuff such as show help message
    //}
}