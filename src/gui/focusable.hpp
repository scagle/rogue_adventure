#pragma once

#include <libtcod/libtcod.hpp>

namespace cursed
{
    class Focusable
    {
        bool focus = false;
        TCODColor focus_color = TCODColor::pink;

        public:
        Focusable();
        virtual ~Focusable() { }

        TCODColor getFocusColor() { return this->focus_color; }
        void setFocus( bool focus ) { this->focus = focus; }
        void toggleFocus() { this->focus ^= true; }
        bool isFocused() { return this->focus; }
    };
};
