#pragma once

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
                return { x + w / 2, y, w / 2, h };
            }
        };

        protected:
        Bound bound;        // LEFT aligned (top left to bottom right) boundary
        TCODColor fg, bg;
        std::string text = "";
        GUI *parent = nullptr;
        std::unique_ptr< GUI > child;
        std::vector< std::unique_ptr< GUI > > children;
        TCOD_alignment_t alignment;

        public:
        GUI( int x, int y, int width, int height );
        virtual ~GUI() { }

        virtual bool getSelectable() { return true; }
        virtual std::string getText() final { return this->text; }
        virtual GUI* getParent() final { return this->parent; }
        virtual TCODColor getSelectedColor() { return TCODColor::lightPurple; }
        virtual TCOD_alignment_t getAlignment() final { return this->alignment; }

        virtual void setText( std::string text ) final { this->text = text; }
        virtual void setAlignment( TCOD_alignment_t alignment ) final { this->alignment = alignment; }
        virtual void setParent( GUI* parent ) final { this->parent = parent; }
        virtual void addChild( std::unique_ptr< GUI > child ) final { this->children.push_back( std::move( child ) ); }
        virtual GUI* getChild( int index ) final { return ( children.size() > index ) ? children[index].get() : nullptr; }
        virtual GUI* getFirstChild() { return ( children.size() > 0 ) ? children[0].get() : nullptr; }

        virtual void performAction() { }
        virtual bool grabNextChild( std::pair< GUI*, int >* focus, int new_index) final;

        virtual void update();
        virtual void render( TCODConsole *console, bool root, GUI* focused_gui );
    };

    class GUIButton : public GUI
    {
        protected:
        void (*action)( GUI* ){}; // Action function pointer
        bool selectable = true;

        public:
        GUIButton( int x, int y, int width, int height );
        virtual ~GUIButton() { }

        bool getSelectable() { return this->selectable; }
        void setAction( void (*action)( GUI* ) ) { this->action = action; }; 

        void performAction( ); 

        virtual void update();
        virtual void render( TCODConsole *console, bool root, GUI* focused_gui );
    };
};
