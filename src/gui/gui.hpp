#pragma once

namespace cursed
{
    class Menu;
};

#include "press_component.hpp"
#include "input_component.hpp"

#include "../enums/menu_action.hpp"

#include <libtcod/libtcod.hpp>

#include <memory>
#include <utility>
#include <vector>

namespace cursed
{
    class GUI
    {
        struct Bound
        {
            int x;
            int y;
            int w;
            int h;

            // Getting sick of converting Left Aligned to Center Aligned coordinates
            Bound getCentered()
            {
                return { x + w / 2, y, w, h };
            }
        };

        protected:
        Menu *menu;         // Menu for which to call actions 
        Bound bound;        // LEFT aligned (top left to bottom right) boundary
        TCODColor fg, bg;
        std::string text = "";
        std::vector< std::unique_ptr< GUI > > children;
        TCOD_alignment_t alignment;
        std::unique_ptr< PressComponent > press_component;
        std::unique_ptr< InputComponent > input_component;

        public:
        GUI( Menu *menu, int x, int y, int width, int height );
        virtual ~GUI() { }

        virtual bool getSelectable() { return true; }
        virtual std::string getText() final { return this->text; }
        virtual TCODColor getSelectedColor() { return TCODColor::lightPurple; }
        virtual TCOD_alignment_t getAlignment() final { return this->alignment; }

        virtual void setText( std::string text ) final { this->text = text; }
        virtual void setAlignment( TCOD_alignment_t alignment ) final { this->alignment = alignment; }

        virtual void setPressComponent( GUI *gui, MenuAction action ) { this->press_component = std::make_unique< PressComponent >( gui, action ); }
        virtual void setInputComponent() { this->input_component = std::make_unique< InputComponent >(); }
        virtual PressComponent* getPressComponent() { return this->press_component.get(); }
        virtual InputComponent* getInputComponent() { return this->input_component.get(); }

        virtual void addChild( std::unique_ptr< GUI > child ) final { this->children.push_back( std::move( child ) ); }
        virtual GUI* getChild( int index ) final { return ( children.size() > index ) ? children[index].get() : nullptr; }
        virtual GUI* getFirstChild() { return ( children.size() > 0 ) ? children[0].get() : nullptr; }
        virtual GUI* getPressableChild( int index );

        virtual void press();
//      virtual void updateInput();

//      virtual bool grabNextChild( std::pair< GUI*, int >* focus, int new_index) final;

        virtual void update();
        virtual void render( TCODConsole *console, bool is_parent, GUI* focused_gui );
    };
};
