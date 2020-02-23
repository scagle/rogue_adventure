#pragma once

#include <string>
#include <libtcod/libtcod.hpp>

namespace cursed
{
    class Textable
    {
        std::string text = "";
        TCOD_alignment_t alignment;
        public:
        Textable();
        virtual ~Textable() { }

        virtual void setText( std::string text ) { this->text = text; }
        virtual void setAlignment( TCOD_alignment_t alignment ) { this->alignment = alignment; }
        virtual std::string getText() { return this->text; }
        virtual TCOD_alignment_t getAlignment() final { return this->alignment; }
    };
};
