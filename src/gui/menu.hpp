#pragma once

#include "gui.hpp"

#include <memory>
#include <utility>
#include <libtcod/libtcod.hpp>

#include <stack>

namespace cursed
{
    class Menu : public TCODConsole
    {
        protected:
        int width, height;
        std::unique_ptr< GUI > root_gui; 
        static std::stack< std::pair< GUI*, int > > gui_stack; 
//      static GUI* last_key_focused_gui; 
//      static GUI* last_mouse_focused_gui; 
        
        public:
        Menu( int width, int height );
        virtual ~Menu() { }

        virtual void init() = 0;

        virtual void update( TCOD_key_t &key, TCOD_mouse_t &mouse ) = 0;
        virtual void render( TCODConsole *console ) = 0;
    };

    class MainMenu : public Menu
    {
        protected:

        public:
        MainMenu( int width, int height );
        virtual ~MainMenu() { }

        virtual void init();
        static bool popGUI();
        static void pushGUI( GUI *gui );
        static void moveFocus( int direction );

        void enterMenu() { if ( gui_stack.empty() ) pushGUI( root_gui.get() ); }

        // Actions
        static void exitMenu( GUI* gui );

        virtual void update( TCOD_key_t &key, TCOD_mouse_t &mouse );
        virtual void render( TCODConsole *console );

    };
};
