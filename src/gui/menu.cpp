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
        setEscapable(false);
        init();
    }

    MainMenu::MainMenu( int width, int height )
        : Menu( width, height )
    {
        setEscapable(false);
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
        gui->addChild( std::move( text ) );
        
        // Agree Button
        std::unique_ptr< GUI > agree = 
            std::make_unique< GUI >( this, width/4, height-3, width/2, 3 );
        agree->setText("\nAgree");
        agree->setPressComponent( nullptr, MenuAction::MAIN_MENU );
        gui->addChild( std::move( agree ) );
    }

    void MainMenu::initMainMenu( GUI *gui )
    {
        int y_offset = 0;
        // New Button
        std::unique_ptr< GUI > character = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        character->setText("\nNew Character");
        character->setPressComponent( character.get(), MenuAction::PUSH );
        initNewCharacter( character.get() );
        gui->addChild( std::move( character ) );

        // Continue Button
        std::unique_ptr< GUI > resume = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        resume->setText("\nContinue");
        resume->setPressComponent( nullptr, MenuAction::RESUME );
        gui->addChild( std::move( resume ) );

        // Load Button
        std::unique_ptr< GUI > load = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        load->setText("\nLoad");
        load->setPressComponent( load.get(), MenuAction::PUSH );
        //initSaveLoad( load.get() );
        gui->addChild( std::move( load ) );

        // Settings
        std::unique_ptr< GUI > settings = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        settings->setText("\nSettings");
        settings->setPressComponent( settings.get(), MenuAction::PUSH );
        //initSettings( settings.get() );
        gui->addChild( std::move( settings ) );

        // Exit
        std::unique_ptr< GUI > exit = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        exit->setText("\nExit Game");
        exit->setPressComponent( nullptr, MenuAction::EXIT );
        gui->addChild( std::move( exit ) );
    }

    void MainMenu::initNewCharacter( GUI *gui )
    {
        int y_offset = 0;
        // Name Input
        y_offset = 6;
        std::unique_ptr< GUI > name_input = 
            std::make_unique< GUI >( this, width/4, y_offset, width/2, 3 );
        name_input->setInputComponent( );

        y_offset = 2;
        std::unique_ptr< GUI > name_option = 
            std::make_unique< GUI >( this, width/4, y_offset, width/2, 3 );
        name_option->setText("\nName");
        name_option->setPressComponent( name_input.get(), MenuAction::FOCUS_INPUT );

        gui->addChild( std::move( name_option ) );
        gui->addChild( std::move( name_input ) );
        y_offset = 7;

        // Back button
        std::unique_ptr< GUI > back = 
            std::make_unique< GUI >( this, width/4, y_offset, width/2, 3 );
        back->setText("\nBack");
        back->setPressComponent( nullptr, MenuAction::POP );
        gui->addChild( std::move( back ) );

    }

    void GameMenu::initGameMenu( GUI *gui )
    {
        int y_offset = 0;
        // Resume Button
        std::unique_ptr< GUI > resume = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        resume->setText("\nResume");
        resume->setPressComponent( nullptr, MenuAction::RESUME );
        gui->addChild( std::move( resume ) );

        // Save / Load Button
        std::unique_ptr< GUI > saveload = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        saveload->setText("\nSave / Load");
        saveload->setPressComponent( saveload.get(), MenuAction::PUSH );
        initSaveLoad( saveload.get() );
        gui->addChild( std::move( saveload ) );

        // Settings
        std::unique_ptr< GUI > settings = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        settings->setText("\nSettings");
        settings->setPressComponent( settings.get(), MenuAction::PUSH );
        initSettings( settings.get() );
        gui->addChild( std::move( settings ) );

        // Exit
        std::unique_ptr< GUI > exit = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        exit->setText("\nExit To Main Menu");
        exit->setPressComponent( nullptr, MenuAction::MAIN_MENU );
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
        initSave( save.get() );
        gui->addChild( std::move( save ) );

        // Load 
        std::unique_ptr< GUI > load = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        load->setText("\nLoad");
        load->setPressComponent( load.get(), MenuAction::PUSH );
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
        initGameSettings( game.get() );
        gui->addChild( std::move( game ) );

        // World Settings
        std::unique_ptr< GUI > world = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        world->setText("\nWorld Settings");
        world->setPressComponent( world.get(), MenuAction::PUSH );
        initWorldSettings( world.get() );
        gui->addChild( std::move( world ) );

        // Content Settings
        std::unique_ptr< GUI > content = 
            std::make_unique< GUI >( this, width/4, y_offset+=3, width/2, 3 );
        content->setText("\nContent Settings");
        content->setPressComponent( content.get(), MenuAction::PUSH );
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
            case MenuAction::EXIT:        // Exit game
                actionExit( origin );
                break;
            case MenuAction::RESUME:      // Resume game
                actionResume( origin );
                break;

            case MenuAction::PUSH:        // Push a GUI to the gui_stack (descend down hierarchy)
                actionPush( origin );
                break;
            case MenuAction::POP:         // Pop a GUI from the gui_stack (ascend up hierarchy)
                actionPop( origin );
                break;
            case MenuAction::FOCUS_INPUT: // Focus specific input above all else
                actionFocusInput( origin );
                break;

            case MenuAction::MAIN_MENU:   // Switch to Main Menu
                actionMainMenu( origin );
                break;

            case MenuAction::SAVE:        // Save Game at Slot #
                actionSave( origin );
                break;
            case MenuAction::LOAD:        // Load Game at Slot #
                actionLoad( origin );
                break;
            default:
                std::printf("*** WARNING: Unknown action! (menu.cpp)\n");
                break;
        }
    }

    void Menu::actionExit( GUI *origin )
    {
        Engine::setState( IDLE );
        Engine::closeGame();
    }

    void Menu::actionResume( GUI *origin )
    {
        while ( popGUI() ) { }
    }

    void Menu::actionPush( GUI *origin )
    {
        pushGUI( origin->getPressComponent()->target );
    }

    void Menu::actionPop( GUI *origin )
    {
        popGUI();
    }

    void Menu::actionFocusInput( GUI *origin )
    {
        std::printf("*** WARNING: WTH do you want me to do!? (menu.cpp)\n");
    }

    void Menu::actionMainMenu( GUI *origin )
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
            if ( escapable || gui_stack.size() > 1 )
            {
                popGUI(); 
            }
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
