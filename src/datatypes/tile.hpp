#pragma once 

#include <libtcod/libtcod.hpp>

namespace cursed
{
    const TCODColor dirt_wall(128, 80, 60);
    const TCODColor dirt_floor(60, 40, 10);

    struct Tile
    {
        int code;
        bool passable;
        TCODColor fg = TCODColor::white;
        TCODColor bg = TCODColor::black;

        Tile() : code( '?' ), passable( true ) {}

        Tile( int code )
        {
            this->code = code;
            this->passable = true; // Default
            switch ( code )
            {
                case ' ' :
                    passable=true;
                    fg = dirt_wall;
                    bg = dirt_floor;
                    break;
                case '#' :
                    passable=false;
                    fg = dirt_wall;
                    bg = dirt_floor;
                    break;
                default:
                    break;
            }
        }

        Tile( const Tile &tile )
        {
            this->code = tile.code;
            this->passable = tile.passable;
            this->fg = tile.fg;
            this->bg = tile.bg;
        }
    };
};
