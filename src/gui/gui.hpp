#pragma once

namespace cursed
{
    class Menu;
};

#include "textable.hpp"
#include "pressable.hpp"
#include "focusable.hpp"

#include "../enums/menu_action.hpp"
#include "../enums/bound.hpp"
#include "../enums/gui_type.hpp"

#include <libtcod/libtcod.hpp>

#include <memory>
#include <utility>
#include <vector>

namespace cursed
{
    class GUI
    {
        protected:
        Menu *menu;         // Menu for which to call actions 
        Bound bound;        // LEFT aligned (top left to bottom right) boundary
        TCODColor fg, bg;
        std::string title = "";
        std::vector< std::unique_ptr< GUI > > children;

        public:
        GUI( Menu *menu, int x, int y, int width, int height );
        virtual ~GUI() { }

        virtual void setTitle(std::string title) final { this->title = title; }
        virtual std::string getTitle() final { return this->title; }

        virtual void addChild( std::unique_ptr< GUI > child ) final { this->children.push_back( std::move( child ) ); }
        virtual GUI* getChild( int index ) final { return ( children.size() > index ) ? children[index].get() : nullptr; }
        virtual GUI* getFirstChild() { return ( children.size() > 0 ) ? children[0].get() : nullptr; }
        virtual GUI* getPressableChild( int index );

        // Code smelly functions
        virtual bool isTextable() { return false; }
        virtual bool isPressable() { return false; }
        virtual bool isInputable() { return false; }
        virtual GUIType getType() { return GUIType::BASE; }

//      virtual void updateInput();

//      virtual bool grabNextChild( std::pair< GUI*, int >* focus, int new_index) final;

        virtual void update();
        virtual void render( TCODConsole *console, bool is_parent, GUI* focused_gui );
    };

    class TextGUI : public GUI, public Textable 
    {
        public:
        TextGUI( Menu *menu, int x, int y, int width, int height );
        virtual ~TextGUI() { }

        virtual GUIType getType() { return GUIType::TEXT; }

        virtual bool isTextable() { return true; }

        virtual void update();
        virtual void render( TCODConsole *console, bool is_parent, GUI* focused_gui );
    };

    class ButtonGUI : public GUI, public Textable, public Pressable
    {
        public:
        ButtonGUI( Menu *menu, int x, int y, int width, int height );
        virtual ~ButtonGUI() { }

        virtual GUIType getType() { return GUIType::BUTTON; }
        virtual void press();

        virtual bool isTextable() { return true; }
        virtual bool isPressable() { return true; };

        virtual void update();
        virtual void render( TCODConsole *console, bool is_parent, GUI* focused_gui );
    };

    class InputGUI : public GUI, public Textable, public Pressable, public Focusable
    {
        public:
        InputGUI( Menu *menu, int x, int y, int width, int height );
        virtual ~InputGUI() { }

        virtual GUIType getType() { return GUIType::INPUT; }

        virtual bool isTextable() { return true; }
        virtual bool isPressable() { return true; };
        virtual bool isInputable() { return true; }

        virtual void update();
        virtual void render( TCODConsole *console, bool is_parent, GUI* focused_gui );
    };
};
