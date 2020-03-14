#pragma once

#include <libtcod/libtcod.hpp>
#include <string>

namespace rogue_adventure
{
    struct Message
    {
        TCODColor color;
        std::string text;

        Message( const TCODColor &color, std::string text ) :
            color( color ), text( text ) { }
        ~Message() { }
    };
};
