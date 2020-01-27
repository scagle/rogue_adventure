#include "menu.hpp"

#include "gui.hpp"
#include "../engine.hpp"
#include "../options.hpp"

#include <stack>
#include <stdlib.h>

namespace cursed
{
    // Static Declaration
//    GUI* Menu::last_key_focused_gui; 
//    GUI* Menu::last_mouse_focused_gui; 
    std::stack< std::pair< GUI*, int > > Menu::gui_stack; 

    // Constructors
    Menu::Menu( int width, int height )
        : TCODConsole( width, height ), width( width ), height( height ) 
    {

    }

    MainMenu::MainMenu( int width, int height )
        : Menu( width, height )
    {
        init();
    }

    // Methods
    void MainMenu::init()
    {
        // Main Menu Screen
        root_gui = std::make_unique< GUI >( 0, 0, width, height );

        int y_offset = 0;
        // Resume Button
        std::unique_ptr< GUIButton > resume = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        resume->setText("Resume");
        resume->setAction( MainMenu::exitMenu );
        resume->setParent( root_gui.get() );
        root_gui->addChild( std::move( resume ) );

        // Save / Load Button
        std::unique_ptr< GUIButton > saveload = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        saveload->setText("Save / Load");
        saveload->setAction( MainMenu::exitMenu );
        saveload->setParent( root_gui.get() );
        root_gui->addChild( std::move( saveload ) );

        // Options
        std::unique_ptr< GUIButton > options = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        options->setText("Options");
        options->setAction( MainMenu::exitMenu );
        options->setParent( root_gui.get() );
        root_gui->addChild( std::move( options ) );

        // Exit
        std::unique_ptr< GUIButton > exit = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        exit->setText("Exit");
        exit->setAction( MainMenu::exitMenu );
        exit->setParent( root_gui.get() );
        root_gui->addChild( std::move( exit ) );


        // Set Default Focus
//      last_key_focused_gui = resume.get(); 
//      last_mouse_focused_gui = resume.get(); 
//      gui_stack.push( { root_gui.get(), 0 } ); 

    }

    void MainMenu::render( TCODConsole *root )
    {
        if ( root )
        {
            // Set default color
            this->setDefaultForeground( TCODColor::white );
            this->setDefaultBackground( TCODColor::black );
            this->setBackgroundFlag( TCOD_BKGND_SET );
            this->setAlignment( TCOD_CENTER );

            // Render all the guis 
            this->printFrame( 0, 0, width, height, true, TCOD_BKGND_DEFAULT, "Main Menu" );
//          this->setDefaultBackground( TCODColor::darkPurple );

            GUI* current_gui = gui_stack.top().first->getChild( gui_stack.top().second );
            if ( current_gui )
            {
                gui_stack.top().first->render( this, true, current_gui );
            }

            // blit the camera to the specified console
            this->blit( this, 0, 0, width, height, root, 0, 0 );
        }
    }

    void MainMenu::update( TCOD_key_t &key, TCOD_mouse_t &mouse )
    {
        TCOD_keycode_t special_key = Engine::getCurrentKey().vk;
        int regular_key = Engine::getCurrentKey().c;
        GUI *current_focused_gui = gui_stack.top().first->getChild( gui_stack.top().second );

        // Special Keys
        if ( special_key == Options::getOptions().MENU )
        {
            popGUI(); 
        }
        else if ( special_key == Options::getOptions().MENU_SELECT )
        {
            if ( current_focused_gui )
            {
                current_focused_gui->performAction();
            }
        }

        // Regular Keys
        if ( current_focused_gui )
        {
            if ( regular_key == Options::getOptions().MENU_DOWN )
            {
                moveFocus( 1 );
            }
            if ( regular_key == Options::getOptions().MENU_UP )
            {
                moveFocus( -1 );
            }
        }
    }

    void MainMenu::moveFocus( int direction )
    {
        if ( gui_stack.top().first->getChild( gui_stack.top().second + direction ) )
        {
            gui_stack.top().second += direction;
            std::printf( "%d\n", gui_stack.top().second );
        }
    } 


    bool MainMenu::popGUI()
    {
        if ( ! gui_stack.empty() )
        {
            gui_stack.pop();
            if ( gui_stack.empty() )
            {
                exitMenu( nullptr );
            }
            return true;
        }

        return false;
    }

    void MainMenu::pushGUI( GUI *gui )
    {
        if ( ! gui )
        {
            std::printf("*** WARNING: Invalid gui (menu.cpp)\n");
            return;
        }
        gui_stack.push( { gui, 0 } );
    }

    void MainMenu::exitMenu( GUI* gui )
    {
        while ( popGUI() ) { }
        Engine::setState( IDLE );
    }
};
