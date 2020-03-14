#include "menu.hpp"

#include "gui.hpp"
#include "../engine.hpp"
#include "../options.hpp"
#include "../enums/gui_type.hpp"

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
        root_gui->setTitle("Agreement");

        initAgreeMenu( root_gui.get() );
    }

    void MainMenu::init()
    {
        // Main Menu Screen
        root_gui = std::make_unique< GUI >( this, 0, 0, width, height );

        // Set names 
        root_gui->setTitle("Main Menu");

        initMainMenu( root_gui.get() );
    }

    void GameMenu::init()
    {
        // Game Menu Screen
        root_gui = std::make_unique< GUI >( this, 0, 0, width, height );

        // Set names 
        root_gui->setTitle("Game Menu");

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

    void AgreeMenu::initAgreeMenu( GUI *parent )
    {
        int y_offset = 0;
        // Content
        std::unique_ptr< TextGUI > text = 
            std::make_unique< TextGUI >( this, 1, 1, width-3, height - 5 );
        text->setText("This is my game it is so great I love everything about it ommmmmgooooshhhhh it is so good this message is such a substantial long message that I hope the text will wrap many many many many times before ending. I need you to agree with me. HERES_A_REALLY_LONG_WORD_I_WANT_TO_SEE_GET_WRAPPED_SOMEHOW_IDK_IF_THIS_WILL_WORK_AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");
        parent->addChild( std::move( text ) );
        
        // Agree Button
        std::unique_ptr< ButtonGUI > agree = 
            std::make_unique< ButtonGUI >( this, width/4, height-3, width/2, 3 );
        agree->setText("\nAgree");
        agree->setAction( MenuAction::MAIN_MENU );
        parent->addChild( std::move( agree ) );
    }

    void MainMenu::initMainMenu( GUI *parent )
    {
        int y_offset = 0;
        // New Button
        std::unique_ptr< ButtonGUI > character = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        character->setText("\nNew Character");
        character->setTitle("Character Creation");
        character->setTarget( character.get() );
        character->setAction( MenuAction::PUSH );
        initNewCharacter( character.get() );
        parent->addChild( std::move( character ) );

        // Continue Button
        std::unique_ptr< ButtonGUI > resume = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        resume->setText("\nContinue");
        resume->setAction( MenuAction::RESUME );
        parent->addChild( std::move( resume ) );

        // Load Button
        std::unique_ptr< ButtonGUI > load = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        load->setText("\nLoad");
        load->setTitle("Load Games");
        load->setTarget( load.get() );
        load->setAction( MenuAction::PUSH );
        //initSaveLoad( load.get() );
        parent->addChild( std::move( load ) );

        // Settings
        std::unique_ptr< ButtonGUI > settings = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        settings->setText("\nSettings");
        settings->setTitle("Settings");
        settings->setTarget( settings.get() );
        settings->setAction( MenuAction::PUSH );
        //initSettings( settings.get() );
        parent->addChild( std::move( settings ) );

        // Exit
        std::unique_ptr< ButtonGUI > exit = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        exit->setText("\nExit Game");
        exit->setAction( MenuAction::EXIT );
        parent->addChild( std::move( exit ) );
    }

    void MainMenu::initNewCharacter( GUI *parent )
    {
        int y_offset = 0;
        GUI* requirement_1 = nullptr;  // Requirement for continue
        // Name Input
        std::unique_ptr< TextInputGUI > name_input = 
            std::make_unique< TextInputGUI >( this, width/4, y_offset+=3, width/2, 3 );
        name_input->setText("\nName");
        requirement_1 = name_input.get();
        parent->addChild( std::move( name_input ) );

        // Strength Input
        std::unique_ptr< SliderGUI > str_input = 
            std::make_unique< SliderGUI >( this, width/4, y_offset+=3, width/2, 3 );
        str_input->setText("\nStrength");
        parent->addChild( std::move( str_input ) );

        // Dexterity Input
        std::unique_ptr< SliderGUI > dex_input = 
            std::make_unique< SliderGUI >( this, width/4, y_offset+=3, width/2, 3 );
        dex_input->setText("\nDexterity");
        parent->addChild( std::move( dex_input ) );

        // Wisdom Input
        std::unique_ptr< SliderGUI > wis_input = 
            std::make_unique< SliderGUI >( this, width/4, y_offset+=3, width/2, 3 );
        wis_input->setText("\nWisdom");
        parent->addChild( std::move( wis_input ) );

        // Continue button
        std::unique_ptr< ButtonGUI > cont = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        cont->setText("\nContinue");
        cont->setTitle("Special Traits");
        cont->setTarget( cont.get() );
        cont->setAction( MenuAction::PUSH );
        cont->addRequirement( requirement_1 );
        initSpecialTraits( cont.get() );
        parent->addChild( std::move( cont ) );

        // Back button
        std::unique_ptr< ButtonGUI > back = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        back->setText("\nBack");
        back->setAction( MenuAction::POP );
        parent->addChild( std::move( back ) );

    }

    void MainMenu::initSpecialTraits( GUI *parent )
    {
        int y_offset = 0;
        GUI* requirement_1 = nullptr;  // Requirement for continue

        // Name Input
        std::unique_ptr< TextInputGUI > name_input = 
            std::make_unique< TextInputGUI >( this, width/4, y_offset+=3, width/2, 3 );
        name_input->setText("\nName");
        requirement_1 = name_input.get();
        parent->addChild( std::move( name_input ) );

        // Continue button
        std::unique_ptr< ButtonGUI > cont = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        cont->setText("\nContinue");
        cont->setTitle("World Settings");
        cont->setTarget( cont.get() );
        cont->setAction( MenuAction::PUSH );
        cont->addRequirement( requirement_1 );
        initWorldSettings( cont.get() );
        parent->addChild( std::move( cont ) );


        // Back button
        std::unique_ptr< ButtonGUI > back = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        back->setText("\nBack");
        back->setAction( MenuAction::POP );
        parent->addChild( std::move( back ) );
    }

    void MainMenu::initWorldSettings( GUI *parent )
    {
        int y_offset = 0;
        // Name Input
        std::unique_ptr< TextInputGUI > name_input = 
            std::make_unique< TextInputGUI >( this, width/4, y_offset+=3, width/2, 3 );
        name_input->setText("\nWorld Name");
        parent->addChild( std::move( name_input ) );

        // Start Button
        std::unique_ptr< ButtonGUI > start = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        start->setText("\nStart");
        start->setAction( MenuAction::RESUME );
        parent->addChild( std::move( start ) );

        // Back button
        std::unique_ptr< ButtonGUI > back = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        back->setText("\nBack");
        back->setAction( MenuAction::POP );
        parent->addChild( std::move( back ) );
    }

    void GameMenu::initGameMenu( GUI *parent )
    {
        int y_offset = 0;
        // Resume Button
        std::unique_ptr< ButtonGUI > resume = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        resume->setText("\nResume");
        resume->setAction( MenuAction::RESUME );
        parent->addChild( std::move( resume ) );

        // Save / Load Button
        std::unique_ptr< ButtonGUI > saveload = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        saveload->setText("\nSave / Load");
        saveload->setTarget( saveload.get() );
        saveload->setAction( MenuAction::PUSH );
        initSaveLoad( saveload.get() );
        parent->addChild( std::move( saveload ) );

        // Settings
        std::unique_ptr< ButtonGUI > settings = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        settings->setText("\nSettings");
        settings->setTarget( settings.get() );
        settings->setAction( MenuAction::PUSH );
        initSettings( settings.get() );
        parent->addChild( std::move( settings ) );

        // Exit
        std::unique_ptr< ButtonGUI > exit = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        exit->setText("\nExit To Main Menu");
        exit->setAction( MenuAction::MAIN_MENU );
        parent->addChild( std::move( exit ) );
    }

    void GameMenu::initSaveLoad( GUI *parent )
    {
        int y_offset = 0;
        // Save 
        std::unique_ptr< ButtonGUI > save = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        save->setText("\nSave");
        save->setTarget( save.get() );
        save->setAction( MenuAction::PUSH );
        initSave( save.get() );
        parent->addChild( std::move( save ) );

        // Load 
        std::unique_ptr< ButtonGUI > load = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        load->setText("\nLoad");
        load->setTarget( load.get() );
        load->setAction( MenuAction::PUSH );
        initLoad( load.get() );
        parent->addChild( std::move( load ) );
    }

    void GameMenu::initSave( GUI *parent )
    {
        int y_offset = 0;
        for ( int i = 0; i < 10; i++ )
        {
            // Save Button
            std::unique_ptr< ButtonGUI > save = 
                std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
            save->setText("\nSave " + std::to_string( i+1 ));
            save->setAction( MenuAction::SAVE );
            parent->addChild( std::move( save ) );
        }
    }

    void GameMenu::initLoad( GUI *parent )
    {
        int y_offset = 0;
        for ( int i = 0; i < 10; i++ )
        {
            // Load Button
            std::unique_ptr< ButtonGUI > load = 
                std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
            load->setText("\nLoad " + std::to_string( i+1 ));
            load->setAction( MenuAction::LOAD );
            parent->addChild( std::move( load ) );
        }
    }
    
    void GameMenu::initSettings( GUI *parent )
    {
        int y_offset = 0;
        // Game Settings
        std::unique_ptr< ButtonGUI > game = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        game->setText("\nGame Settings");
        game->setTarget( game.get() );
        game->setAction( MenuAction::PUSH );
        initGameSettings( game.get() );
        parent->addChild( std::move( game ) );

        // World Settings
        std::unique_ptr< ButtonGUI > world = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        world->setText("\nWorld Settings");
        world->setTarget( world.get() );
        world->setAction( MenuAction::PUSH );
        initWorldSettings( world.get() );
        parent->addChild( std::move( world ) );

        // Content Settings
        std::unique_ptr< ButtonGUI > content = 
            std::make_unique< ButtonGUI >( this, width/4, y_offset+=3, width/2, 3 );
        content->setText("\nContent Settings");
        content->setTarget( content.get() );
        content->setAction( MenuAction::PUSH );
        initContentSettings( content.get() );
        parent->addChild( std::move( content ) );
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
    void Menu::action( ButtonGUI *origin, MenuAction menu_action )
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

    void Menu::actionExit( ButtonGUI *origin )
    {
        Engine::setState( IDLE );
        Engine::closeGame();
    }

    void Menu::actionResume( ButtonGUI *origin )
    {
        while ( popGUI() ) { }
    }

    void Menu::actionPush( ButtonGUI *origin )
    {
        pushGUI( origin->getTarget() );
    }

    void Menu::actionPop( ButtonGUI *origin )
    {
        popGUI();
    }

    void Menu::actionFocusInput( ButtonGUI *origin )
    {
        std::printf("*** WARNING: WTH do you want me to do!? (menu.cpp)\n");
    }

    void Menu::actionMainMenu( ButtonGUI *origin )
    {
        emit( ButtonEvent::MAIN_MENU );
    }

    void Menu::actionSave( ButtonGUI *origin )
    {
        std::printf("Saving '%s'\n", origin->getText().c_str());
    }

    void Menu::actionLoad( ButtonGUI *origin )
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
        if ( special_key == Options::getOptions().MENU_SELECT )
        {
            if ( current_focused_gui )
            {
                current_focused_gui->select();
            }
        }

        if ( current_focused_gui )
        {
            if ( current_focused_gui->isFocusable() && current_focused_gui->isFocused() )
            {            
                current_focused_gui->update( key, mouse );
                return;
            }
        }

        if ( special_key == Options::getOptions().MENU )
        {
            if ( escapable || gui_stack.size() > 1 )
            {
                popGUI(); 
            }
        }

        if ( current_focused_gui )
        {
            // Regular Keys
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
