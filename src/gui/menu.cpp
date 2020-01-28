#include "menu.hpp"

#include "gui.hpp"
#include "../engine.hpp"
#include "../options.hpp"

#include <stack>
#include <stdlib.h>

namespace cursed
{
    // Static Declaration
    std::vector< Menu* > Menu::instances;
    Menu *Menu::current_focused_menu;
    std::stack< std::pair< GUI*, int > > Menu::gui_stack; 
    std::unique_ptr< GUI > Menu::root_gui; 

    // Constructors
    Menu::Menu( int width, int height )
        : TCODConsole( width, height ), width( width ), height( height ) 
    {
        instances.push_back( this ); 
    }

    AgreeMenu::AgreeMenu( int width, int height )
        : Menu( width, height )
    {
        init();
    }

    MainMenu::MainMenu( int width, int height )
        : Menu( width, height )
    {
        init();
    }

    GameMenu::GameMenu( int width, int height )
        : Menu( width, height )
    {
        init();
    }

    // Methods
    void AgreeMenu::init()
    {
        // Game Menu Screen
        root_gui = std::make_unique< GUI >( 0, 0, width, height );

        // Set names 
        root_gui->setText("Agreement");

//      initAgreeMenu( root_gui.get() );
    }

    void MainMenu::init()
    {
        // Main Menu Screen
        root_gui = std::make_unique< GUI >( 0, 0, width, height );

        // Set names 
        root_gui->setText("Main Menu");

//      initMainMenu( root_gui.get() );
    }

    void GameMenu::init()
    {
        // Game Menu Screen
        root_gui = std::make_unique< GUI >( 0, 0, width, height );

        // Set names 
        root_gui->setText("Game Menu");

        initGameMenu( root_gui.get() );
    }

    void AgreeMenu::listen( ListenEvent event )
    {
    }

    void MainMenu::listen( ListenEvent event )
    {
        if ( event == MAIN_MENU )
        {
            switchCurrentMenu( this );
        }
    }

    void GameMenu::listen( ListenEvent event )
    {
    }

    void Menu::emit( ListenEvent event )
    {
        for ( auto* instance : instances )
        {
            instance->listen( event );
        }
    }

    void AgreeMenu::initAgreeMenu( GUI *gui )
    {
        int y_offset = 0;
        // Content
        std::unique_ptr< GUI > text = 
            std::make_unique< GUIButton >( width, 0, width/2, height - 3 );
        text->setText("This is my game it is so great I love everything about it ommmmmgooooshhhhh it is so good this message is such a substantial long message that I hope the text will wrap many many many many times before ending. I need you to agree with me.");
        text->setParent( gui );
        gui->addChild( std::move( text ) );
        
        // Agree Button
        std::unique_ptr< GUIButton > agree = 
            std::make_unique< GUIButton >( width/4, height-3, width/2, 3 );
        agree->setText("\nAgree");
        agree->setAction( Menu::actionMenu );
        agree->setParent( gui );
        gui->addChild( std::move( agree ) );
    }

    void GameMenu::initGameMenu( GUI *gui )
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

        // Settings
        std::unique_ptr< GUIButton > settings = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        settings->setText("\nSettings");
        settings->setAction( Menu::actionPush );
        settings->setPush( settings.get() );
        settings->setParent( gui );
        initSettings( settings.get() );
        gui->addChild( std::move( settings ) );

        // Exit
        std::unique_ptr< GUIButton > exit = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        exit->setText("\nExit");
        exit->setAction( Menu::actionExit );
        exit->setParent( gui );
        gui->addChild( std::move( exit ) );

    }

    void GameMenu::initSaveLoad( GUI *gui )
    {
        int y_offset = 0;
        // Save 
        std::unique_ptr< GUIButton > save = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        save->setText("\nSave");
        save->setAction( Menu::actionPush );
        save->setPush( save.get() );
        save->setParent( gui );
        initSave( save.get() );
        gui->addChild( std::move( save ) );

        // Load 
        std::unique_ptr< GUIButton > load = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        load->setText("\nLoad");
        load->setAction( Menu::actionPush );
        load->setPush( load.get() );
        load->setParent( gui );
        initLoad( load.get() );
        gui->addChild( std::move( load ) );

    }

    void GameMenu::initSave( GUI *gui )
    {
        int y_offset = 0;
        for ( int i = 0; i < 10; i++ )
        {
            // Save Button
            std::unique_ptr< GUIButton > save = 
                std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
            save->setText("\nSave " + std::to_string( i+1 ));
            save->setAction( Menu::actionSave );
            save->setParent( gui );
            gui->addChild( std::move( save ) );
        }
    }

    void GameMenu::initLoad( GUI *gui )
    {
        int y_offset = 0;
        for ( int i = 0; i < 10; i++ )
        {
            // Load Button
            std::unique_ptr< GUIButton > load = 
                std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
            load->setText("\nLoad " + std::to_string( i+1 ));
            load->setAction( Menu::actionLoad );
            load->setParent( gui );
            gui->addChild( std::move( load ) );
        }
    }
    
    void GameMenu::initSettings( GUI *gui )
    {
        int y_offset = 0;
        // Game Settings
        std::unique_ptr< GUIButton > game = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        game->setText("\nGame Settings");
        game->setAction( Menu::actionPush );
        game->setPush( game.get() );
        game->setParent( gui );
        initGameSettings( game.get() );
        gui->addChild( std::move( game ) );

        // World Settings
        std::unique_ptr< GUIButton > world = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        world->setText("\nWorld Settings");
        world->setAction( Menu::actionPush );
        world->setPush( world.get() );
        world->setParent( gui );
        initWorldSettings( world.get() );
        gui->addChild( std::move( world ) );

        // Content Settings
        std::unique_ptr< GUIButton > content = 
            std::make_unique< GUIButton >( width/4, y_offset+=3, width/2, 3 );
        content->setText("\nContent Settings");
        content->setAction( Menu::actionPush );
        content->setPush( content.get() );
        content->setParent( gui );
        initContentSettings( content.get() );
        gui->addChild( std::move( content ) );

    }

    void GameMenu::initGameSettings( GUI *parent )
    {

    }

    void GameMenu::initWorldSettings( GUI *parent )
    {

    }

    void GameMenu::initContentSettings( GUI *parent )
    {

    }

    void Menu::moveFocus( int direction )
    {
        if ( gui_stack.top().first->getChild( gui_stack.top().second + direction ) )
        {
            gui_stack.top().second += direction;
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

    void Menu::actionMenu( GUI *origin )
    {
        emit( MAIN_MENU );
    }

    void Menu::actionSave( GUI *origin )
    {
        std::printf("Saving '%s'\n", origin->getText().c_str());
    }

    void Menu::actionLoad( GUI *origin )
    {
        std::printf("Loading from '%s'\n", origin->getText().c_str());
    }

    void AgreeMenu::render( TCODConsole *root )
    {/* TODO: Implement this! */}

    void AgreeMenu::update( TCOD_key_t &key, TCOD_mouse_t &mouse )
    {/* TODO: Implement this! */}

    void MainMenu::render( TCODConsole *root )
    {/* TODO: Implement this! */}

    void MainMenu::update( TCOD_key_t &key, TCOD_mouse_t &mouse )
    {/* TODO: Implement this! */}

    void GameMenu::render( TCODConsole *root )
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

    void GameMenu::update( TCOD_key_t &key, TCOD_mouse_t &mouse )
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

    void Menu::switchCurrentMenu( Menu *menu )
    {
        // Clear current menu
        if ( current_focused_menu )
        {
            while ( current_focused_menu->popGUI() ) { }
        }

        // Clear next menu (Just in case), and switch to it
        while ( menu->popGUI() ) { }
        menu->enterMenu();
        current_focused_menu = menu;
    }

    void Menu::updateCurrentMenu( TCOD_key_t &key, TCOD_mouse_t &mouse )
    {
        current_focused_menu->update( key, mouse );
    }

    void Menu::renderCurrentMenu( TCODConsole *console )
    {
        current_focused_menu->render( console );
    }
};
