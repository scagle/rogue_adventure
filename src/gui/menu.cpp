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
    std::pair< GUI*, int > Menu::current_focused_gui; 
    std::stack< GUI* > Menu::gui_stack; 

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
        current_focused_gui = { root_gui->getFirstChild(), 0 }; 

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

            if ( gui_stack.empty() )
            {
                root_gui->render( this, current_focused_gui.first ); // Render main menu
            }
            else
            {
                gui_stack.top()->render( this, current_focused_gui.first );
            }

            // blit the camera to the specified console
            this->blit( this, 0, 0, width, height, root, 0, 0 );
        }
    }

    void MainMenu::update( TCOD_key_t &key, TCOD_mouse_t &mouse )
    {
        TCOD_keycode_t special_key = Engine::getCurrentKey().vk;
        int regular_key = Engine::getCurrentKey().c;

        // Special Keys
        if ( special_key == Options::getOptions().MENU )
        {
            if ( gui_stack.empty() )
            {
                Engine::setState( IDLE );
            }
            else
            {
                gui_stack.pop();
            }
        }
        else if ( special_key == Options::getOptions().MENU_SELECT )
        {
            if ( current_focused_gui.first )
            {
                current_focused_gui.first->performAction();
            }
        }

        // Regular Keys
        if ( current_focused_gui.first )
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
        GUI *parent = current_focused_gui.first->getParent();
        int new_index = current_focused_gui.second + direction;
        parent->grabNextChild( &current_focused_gui, new_index );
    } 

    bool MainMenu::popGUI()
    {
        if ( ! gui_stack.empty() )
        {
            gui_stack.pop();
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
        gui_stack.push( gui );
        current_focused_gui = { gui->getFirstChild(), 0 };
        if ( current_focused_gui.first )
            std::printf("Focusing: %s\n", current_focused_gui.first->getText().c_str());
        else
            std::printf("Focusing: None\n");
    }

    void MainMenu::exitMenu( GUI* gui )
    {
        while ( popGUI() ) { }
        Engine::setState( IDLE );
    }
};
