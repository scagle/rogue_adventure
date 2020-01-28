#pragma once

#include "gui.hpp"

#include <memory>
#include <utility>
#include <libtcod/libtcod.hpp>

#include <stack>

namespace cursed
{
    enum ListenEvent
    {
        MAIN_MENU,
    };

    class Menu : public TCODConsole
    {
        // TODO: De staticify this class
        private:
        static std::vector< Menu* > instances;
        static Menu *current_focused_menu;
        
        protected:
        int width, height;
        static std::unique_ptr< GUI > root_gui; 
        static std::stack< std::pair< GUI*, int > > gui_stack; // Stack of GUI's and their button indices
        
        public:
        Menu( int width, int height );
        virtual ~Menu() { }

        virtual void init() = 0;
        virtual void listen( ListenEvent event ) = 0; // Listen for events
        static void emit( ListenEvent event ); // Emit events

        static void actionResume( GUI* origin );
        static void actionPush( GUI* origin );
        static void actionExit( GUI* origin );
        static void actionSave( GUI* origin );
        static void actionLoad( GUI* origin );
        static void actionMenu( GUI* origin );

        static bool popGUI();
        static void pushGUI( GUI *gui );
        static void moveFocus( int direction );

        static void enterMenu() { if ( gui_stack.empty() ) pushGUI( root_gui.get() ); }
        static void exitMenu( GUI* gui );

        bool isEmpty() { return ( this->gui_stack.size() == 0 ); }

        static void switchCurrentMenu( Menu *menu );  // Switch to new menu
        static void updateCurrentMenu( TCOD_key_t &key, TCOD_mouse_t &mouse );
        static void renderCurrentMenu( TCODConsole *console );

        virtual void update( TCOD_key_t &key, TCOD_mouse_t &mouse ) = 0;
        virtual void render( TCODConsole *console ) = 0;
    };

    class AgreeMenu : public Menu
    {
        protected:

        public:
        AgreeMenu( int width, int height );
        virtual ~AgreeMenu() { }

        virtual void init();
        virtual void listen( ListenEvent event ); // Listen for events

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

        virtual void init();
        virtual void listen( ListenEvent event ); // Listen for events

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
        virtual void listen( ListenEvent event ); // Listen for events

        void initMainMenu( GUI *parent );
        void initGameMenu( GUI *parent );

        void initSaveLoad( GUI *parent );
        void initSave( GUI *parent );
        void initLoad( GUI *parent );

        void initSettings( GUI *parent );
        void initGameSettings( GUI *parent );
        void initWorldSettings( GUI *parent );
        void initContentSettings( GUI *parent );

        virtual void update( TCOD_key_t &key, TCOD_mouse_t &mouse );
        virtual void render( TCODConsole *console );

    };
};
