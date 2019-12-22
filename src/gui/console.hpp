#pragma once

#include "../datatypes/message.hpp"

#include <libtcod/libtcod.hpp>
#include <fmt/format.h>
#include <utility>
#include <vector>

namespace cursed
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

        void message( const TCODColor &color, const char *text, ... );

        protected:
        void renderBar( int x, int y, int width, std::string name, 
            float value, float max_value, const TCODColor &bar_color,
            const TCODColor &back_color );
        void renderMouseLook();

    };
};
