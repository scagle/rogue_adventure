#pragma once

#include "../datatypes/message.hpp"

#include <libtcod/libtcod.hpp>
#include <fmt/format.h>
#include <utility>
#include <vector>

namespace rogue_adventure
{
    const int BAR_WIDTH = 20;
    const int MAX_MSG_SIZE = 50;

    class Console
    {
        protected:
        int width;
        int height;
        std::unique_ptr< TCODConsole > console;
        std::vector< Message > log;

        public:
        Console( int panel_width, int panel_height );
        virtual ~Console() { }

        void render();

        // Message to console, and its helper wrappers
        void message( const TCODColor &color, std::string text, ... );
        void message( const TCODColor &color, const char *text, ... );
        void message( const TCODColor &color, const char *text, va_list args );

        protected:
        void renderBar( int x, int y, int width, std::string name, 
            float value, float max_value, const TCODColor &bar_color,
            const TCODColor &back_color );
        void renderMouseLook();
    };
};
