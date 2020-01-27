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
    std::unique_ptr< GUI > Menu::root_gui; 

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

        // Set names 
        root_gui->setText("Main Menu");

        initMainMenu( root_gui.get() );

        // Set Default Focus
//      last_key_focused_gui = resume.get(); 
//      last_mouse_focused_gui = resume.get(); 
//      current_focused_gui = { root_gui->getFirstChild(), 0 }; 

    }

    void MainMenu::initMainMenu( GUI *gui )
    {
        int y_offset = 0;
        // Resume Button
        std::unique_ptr< GUIButton > resume = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        resume->setText("\nResume");
        resume->setAction( Menu::actionResume );
        resume->setParent( gui );
        gui->addChild( std::move( resume ) );

        // Save / Load Button
        std::unique_ptr< GUIButton > saveload = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        saveload->setText("\nSave / Load");
        saveload->setAction( Menu::actionPush );
        saveload->setPush( saveload.get() );
        saveload->setParent( gui );
        initSaveLoad( saveload.get() );
        gui->addChild( std::move( saveload ) );

        // Options
        std::unique_ptr< GUIButton > options = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        options->setText("\nOptions");
        options->setAction( Menu::actionPush );
        options->setPush( options.get() );
        options->setParent( gui );
        initOptions( options.get() );
        gui->addChild( std::move( options ) );

        // Exit
        std::unique_ptr< GUIButton > exit = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        exit->setText("\nExit");
        exit->setAction( Menu::actionExit );
        exit->setParent( gui );
        gui->addChild( std::move( exit ) );

    }

    void MainMenu::initSaveLoad( GUI *gui )
    {

    }
    
    void MainMenu::initOptions( GUI *gui )
    {
        int y_offset = 0;
        // Button
        std::unique_ptr< GUIButton > test1 = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        test1->setText("\nTest 1");
        test1->setAction( Menu::actionPush );
        test1->setPush( test1.get() );
        test1->setParent( gui );
        gui->addChild( std::move( test1 ) );

        // Button
        std::unique_ptr< GUIButton > test2 = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        test2->setText("\nTest 2");
        test2->setAction( Menu::actionPush );
        test2->setPush( test2.get() );
        test2->setParent( gui );
        gui->addChild( std::move( test2 ) );

        // Button
        std::unique_ptr< GUIButton > test3 = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        test3->setText("\nTest 3");
        test3->setAction( Menu::actionPush );
        test3->setPush( test3.get() );
        test3->setParent( gui );
        gui->addChild( std::move( test3 ) );
    }

    void Menu::moveFocus( int direction )
    {
        if ( gui_stack.top().first->getChild( gui_stack.top().second + direction ) )
        {
            gui_stack.top().second += direction;
            std::printf( "%d\n", gui_stack.top().second );
        }
    } 

    bool Menu::popGUI()
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

    void Menu::pushGUI( GUI *gui )
    {
        if ( ! gui )
        {
            std::printf("*** WARNING: Invalid gui (menu.cpp)\n");
            return;
        }
        gui_stack.push( { gui, 0 } );
    }

    void Menu::exitMenu( GUI* gui )
    {
        while ( popGUI() ) { }
        Engine::setState( IDLE );
    }

    // Actions
    void Menu::actionResume( GUI *origin )
    {
        while ( popGUI() ) { }
    }

    void Menu::actionPush( GUI *origin )
    {
        pushGUI( origin->getPush() );
    }

    void Menu::actionExit( GUI *origin )
    {
        Engine::setState( IDLE );
        Engine::closeGame();
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

};
