#include "menu.hpp"

#include "gui.hpp"
#include "../engine.hpp"
#include "../options.hpp"

#include <stack>
#include <stdlib.h>

namespace cursed
{
    // Static Declaration
    Menu *Menu::current_focused_menu;

    // Constructors
    Menu::Menu( int width, int height )
        : TCODConsole( width, height ), width( width ), height( height ) 
    {
        getInstances().push_back( this ); 
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

    std::vector< Menu* >& Menu::getInstances()
    {
      static std::vector< Menu* > instances;
      return instances;
    }

    // Methods
    void AgreeMenu::init()
    {
        // Game Menu Screen
        root_gui = std::make_unique< GUI >( this, 0, 0, width, height );

        // Set names 
        root_gui->setText("Agreement");

        initAgreeMenu( root_gui.get() );
    }

    void MainMenu::init()
    {
        // Main Menu Screen
        root_gui = std::make_unique< GUI >( this, 0, 0, width, height );

        // Set names 
        root_gui->setText("Main Menu");

        initMainMenu( root_gui.get() );
    }

    void GameMenu::init()
    {
        // Game Menu Screen
        root_gui = std::make_unique< GUI >( this, 0, 0, width, height );

        // Set names 
        root_gui->setText("Game Menu");

        initGameMenu( root_gui.get() );
    }

    void AgreeMenu::listen( ButtonEvent event )
    {

    }

    void MainMenu::listen( ButtonEvent event )
    {
        if ( event == ButtonEvent::MAIN_MENU )
        {
            switchCurrentMenu( this );
            std::printf( "Switching to Main Menu (%p)\n", this );
        }
    }

    void GameMenu::listen( ButtonEvent event )
    {

    }

    void Menu::emit( ButtonEvent event )
    {
        for ( auto* instance : getInstances() )
        {
            instance->listen( event );
        }
    }

    void AgreeMenu::initAgreeMenu( GUI *gui )
    {
        int y_offset = 0;
        // Content
        std::unique_ptr< GUI > text = 
            std::make_unique< GUI >( this, 1, 1, width-3, height - 5 );
        text->setText("This is my game it is so great I love everything about it ommmmmgooooshhhhh it is so good this message is such a substantial long message that I hope the text will wrap many many many many times before ending. I need you to agree with me. HERES_A_REALLY_LONG_WORD_I_WANT_TO_SEE_GET_WRAPPED_SOMEHOW_IDK_IF_THIS_WILL_WORK_AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");
        text->setParent( gui );
        gui->addChild( std::move( text ) );
        
        // Agree Button
        std::unique_ptr< GUI > agree = 
            std::make_unique< GUI >( this, width/4, height-3, width/2, 3 );
        agree->setText("\nAgree");
        agree->setPressComponent( nullptr, MenuAction::MENU );
        agree->setParent( gui );
        gui->addChild( std::move( agree ) );
    }

    void MainMenu::initMainMenu( GUI *gui )
    {
        int y_offset = 0;
        // New Button
        std::unique_ptr< GUI > character = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        character->setText("\nNew Character");
        character->setPressComponent( nullptr, MenuAction::NEW_CHARACTER );
        character->setParent( gui );
        gui->addChild( std::move( character ) );

        // Continue Button
        std::unique_ptr< GUI > resume = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        resume->setText("\nContinue");
        resume->setPressComponent( nullptr, MenuAction::RESUME );
        resume->setParent( gui );
        gui->addChild( std::move( resume ) );

        // Load Button
        std::unique_ptr< GUI > load = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        load->setText("\nLoad");
        load->setPressComponent( load.get(), MenuAction::PUSH );
        load->setParent( gui );
        //initSaveLoad( load.get() );
        gui->addChild( std::move( load ) );

        // Settings
        std::unique_ptr< GUI > settings = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        settings->setText("\nSettings");
        settings->setPressComponent( settings.get(), MenuAction::PUSH );
        settings->setParent( gui );
        //initSettings( settings.get() );
        gui->addChild( std::move( settings ) );

        // Exit
        std::unique_ptr< GUI > exit = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        exit->setText("\nExit Game");
        exit->setPressComponent( nullptr, MenuAction::EXIT );
        exit->setParent( gui );
        gui->addChild( std::move( exit ) );
    }

    void GameMenu::initGameMenu( GUI *gui )
    {
        int y_offset = 0;
        // Resume Button
        std::unique_ptr< GUI > resume = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        resume->setText("\nResume");
        resume->setPressComponent( nullptr, MenuAction::RESUME );
        resume->setParent( gui );
        gui->addChild( std::move( resume ) );

        // Save / Load Button
        std::unique_ptr< GUI > saveload = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        saveload->setText("\nSave / Load");
        saveload->setPressComponent( saveload.get(), MenuAction::PUSH );
        saveload->setParent( gui );
        initSaveLoad( saveload.get() );
        gui->addChild( std::move( saveload ) );

        // Settings
        std::unique_ptr< GUI > settings = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        settings->setText("\nSettings");
        settings->setPressComponent( settings.get(), MenuAction::PUSH );
        settings->setParent( gui );
        initSettings( settings.get() );
        gui->addChild( std::move( settings ) );

        // Exit
        std::unique_ptr< GUI > exit = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        exit->setText("\nExit To Main Menu");
        exit->setPressComponent( nullptr, MenuAction::MENU );
        exit->setParent( gui );
        gui->addChild( std::move( exit ) );
    }

    void GameMenu::initSaveLoad( GUI *gui )
    {
        int y_offset = 0;
        // Save 
        std::unique_ptr< GUI > save = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        save->setText("\nSave");
        save->setPressComponent( save.get(), MenuAction::PUSH );
        save->setParent( gui );
        initSave( save.get() );
        gui->addChild( std::move( save ) );

        // Load 
        std::unique_ptr< GUI > load = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        load->setText("\nLoad");
        load->setPressComponent( load.get(), MenuAction::PUSH );
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
            std::unique_ptr< GUI > save = 
                std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
            save->setText("\nSave " + std::to_string( i+1 ));
            save->setPressComponent( nullptr, MenuAction::SAVE );
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
            std::unique_ptr< GUI > load = 
                std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
            load->setText("\nLoad " + std::to_string( i+1 ));
            load->setPressComponent( nullptr, MenuAction::LOAD );
            load->setParent( gui );
            gui->addChild( std::move( load ) );
        }
    }
    
    void GameMenu::initSettings( GUI *gui )
    {
        int y_offset = 0;
        // Game Settings
        std::unique_ptr< GUI > game = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        game->setText("\nGame Settings");
        game->setPressComponent( game.get(), MenuAction::PUSH );
        game->setParent( gui );
        initGameSettings( game.get() );
        gui->addChild( std::move( game ) );

        // World Settings
        std::unique_ptr< GUI > world = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        world->setText("\nWorld Settings");
        world->setPressComponent( world.get(), MenuAction::PUSH );
        world->setParent( gui );
        initWorldSettings( world.get() );
        gui->addChild( std::move( world ) );

        // Content Settings
        std::unique_ptr< GUI > content = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        content->setText("\nContent Settings");
        content->setPressComponent( content.get(), MenuAction::PUSH );
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
        GUI *top = gui_stack.top().first;
        int *index = &(gui_stack.top().second);
        if ( top->getPressableChild( *index + direction ) )
        {
            *index += direction;
        }
//      if ( top->getChild( *index + direction ) )
//      { 
//          if ( top->getChild( *index + direction )->getPressComponent() )
//          {
//               *index += direction;
//          }
//          else
//          {
//              // Continue on until either a pressable button or edge boundary is reached
//              int new_index = *index + ( direction * 2 );
//              GUI *new_child = top->getChild( new_index );
//              while ( new_index < top->getChildrenSize() && top->getChild( new_index )->getPressComponent() == nullptr )
//              {
//                  new_child = top->getChild( new_index );
//                  new_index++;
//              }
//
//              // If we didn't reach a boundary, than it must be a pressable button
//              if ( new_index < top->getChildrenSize() )
//              {
//                  *index = new_index;
//              }
//          }
//      }
    } 

    bool Menu::popGUI( bool prevent_exit )
    {
        if ( ! gui_stack.empty() )
        {
            gui_stack.pop();
            if ( gui_stack.empty() && ! prevent_exit )
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

    void Menu::enterMenu()
    { 
        if ( gui_stack.empty() ) 
            pushGUI( root_gui.get() ); 
    }

    void Menu::exitMenu( GUI* gui )
    {
//      while ( popGUI() ) { }
        Engine::setState( IDLE );
    }

    // Actions
    void Menu::action( GUI *origin, MenuAction menu_action )
    {
        switch ( menu_action )
        {
            case MenuAction::RESUME:
                actionResume( origin );
                break;
            case MenuAction::PUSH:
                actionPush( origin );
                break;
            case MenuAction::EXIT:
                actionExit( origin );
                break;
            case MenuAction::MENU:
                actionMenu( origin );
                break;
            case MenuAction::SAVE:
                actionSave( origin );
                break;
            case MenuAction::LOAD:
                actionLoad( origin );
                break;
            default:
                std::printf("*** WARNING: Unknown action! (menu.cpp)\n");
                break;
        }
    }

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
        emit( ButtonEvent::MAIN_MENU );
    }

    void Menu::actionSave( GUI *origin )
    {
        std::printf("Saving '%s'\n", origin->getText().c_str());
    }

    void Menu::actionLoad( GUI *origin )
    {
        std::printf("Loading from '%s'\n", origin->getText().c_str());
    }

    void AgreeMenu::update( TCOD_key_t &key, TCOD_mouse_t &mouse )
    {
        Menu::update( key, mouse );
    }

    void AgreeMenu::render( TCODConsole *root )
    {
        Menu::render( root );
    }

    void MainMenu::update( TCOD_key_t &key, TCOD_mouse_t &mouse )
    {
        Menu::update( key, mouse );
    }

    void MainMenu::render( TCODConsole *root )
    {
        Menu::render( root );
    }

    void GameMenu::update( TCOD_key_t &key, TCOD_mouse_t &mouse )
    {
        Menu::update( key, mouse );
    }

    void GameMenu::render( TCODConsole *root )
    {
        Menu::render( root );
    }

    void Menu::render( TCODConsole *root )
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

            GUI* current_gui = gui_stack.top().first->getPressableChild( gui_stack.top().second );
            if ( current_gui )
            {
                gui_stack.top().first->render( this, true, current_gui );
            }

            // blit the camera to the specified console
            this->blit( this, 0, 0, width, height, root, 0, 0 );
        }

    }

    void Menu::update( TCOD_key_t &key, TCOD_mouse_t &mouse )
    {
        TCOD_keycode_t special_key = Engine::getCurrentKey().vk;
        int regular_key = Engine::getCurrentKey().c;
        GUI *current_focused_gui = gui_stack.top().first->getPressableChild( gui_stack.top().second );

        // Special Keys
        if ( special_key == Options::getOptions().MENU )
        {
            popGUI(); 
        }
        else if ( special_key == Options::getOptions().MENU_SELECT )
        {
            if ( current_focused_gui )
            {
                current_focused_gui->press();
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
            while ( current_focused_menu->popGUI(true) ) { }
        }

        // Clear next menu (Just in case), and switch to it
        // while ( menu->popGUI() ) { }
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
