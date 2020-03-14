#pragma once

#include "gui.hpp"

#include "../enums/menu_action.hpp"
#include "../enums/menu_type.hpp"
#include "../enums/button_event.hpp"

#include <memory>
#include <utility>
#include <libtcod/libtcod.hpp>

#include <stack>

namespace cursed
{
    class Menu : public TCODConsole
    {
        public:
        static Menu *current_focused_menu;
        
        protected:
        int width, height;
        std::unique_ptr< GUI > root_gui; 
        std::stack< std::pair< GUI*, int > > gui_stack; // Stack of GUI's and their button indices
        bool escapable = true;
        
        public:
        Menu( int width, int height );
        virtual ~Menu() { }

        static std::vector< Menu* >& getInstances(); // Static Order Initialization Fiasco fix

        virtual void init() = 0;
        virtual void listen( ButtonEvent event ) = 0; // Listen for events
        static void emit( ButtonEvent event ); // Emit events

        void action( ButtonGUI *origin, MenuAction menu_action );
        void actionExit( ButtonGUI* origin );
        void actionResume( ButtonGUI* origin );
        void actionPush( ButtonGUI* origin );
        void actionPop( ButtonGUI* origin );
        void actionFocusInput( ButtonGUI* origin );
        void actionMainMenu( ButtonGUI* origin );
        void actionSave( ButtonGUI* origin );
        void actionLoad( ButtonGUI* origin );

        bool popGUI( bool prevent_exit = false );
        void pushGUI( GUI *gui );
        void moveFocus( int direction );

        void enterMenu(); 
        void exitMenu( GUI* gui );

        bool isEmpty() { return ( this->gui_stack.size() == 0 ); }
        void setEscapable( bool escapable ) { this->escapable = escapable; }

        static void switchCurrentMenu( Menu *menu );  // Switch to new menu
        static void updateCurrentMenu( TCOD_key_t &key, TCOD_mouse_t &mouse );
        static void renderCurrentMenu( TCODConsole *console );

        protected:
        virtual void update( TCOD_key_t &key, TCOD_mouse_t &mouse );
        virtual void render( TCODConsole *console );
    };

    class AgreeMenu : public Menu
    {
        protected:

        public:
        AgreeMenu( int width, int height );
        virtual ~AgreeMenu() { }

        virtual void init();
        virtual void listen( ButtonEvent event ); // Listen for events

        void initAgreeMenu( GUI *parent );

        virtual void update( TCOD_key_t &key, TCOD_mouse_t &mouse );
        virtual void render( TCODConsole *console );
    };

    class MainMenu : public Menu
    {
        protected:

        public:
        MainMenu( int width, int height );
        virtual ~MainMenu() { }

        void initMainMenu( GUI *parent );
        void initNewCharacter( GUI *parent );
        void initSpecialTraits( GUI *parent );
        void initWorldSettings( GUI *parent );

        virtual void init();
        virtual void listen( ButtonEvent event ); // Listen for events

        virtual void update( TCOD_key_t &key, TCOD_mouse_t &mouse );
        virtual void render( TCODConsole *console );
    };

    class GameMenu : public Menu
    {
        protected:

        public:
        GameMenu( int width, int height );
        virtual ~GameMenu() { }

        virtual void init();
        virtual void listen( ButtonEvent event ); // Listen for events

        void initGameMenu( GUI *parent );

        void initSaveLoad( GUI *parent );
        void initSave( GUI *parent );
        void initLoad( GUI *parent );

        void initSettings( GUI *parent );
        void initGameSettings( GUI *parent );
        void initWorldSettings( GUI *parent );
        void initContentSettings( GUI *parent );

        protected:
        virtual void update( TCOD_key_t &key, TCOD_mouse_t &mouse );
        virtual void render( TCODConsole *console );

    };
};
