#pragma once 

#include "../enums/tile_type.hpp"
#include <libtcod/libtcod.hpp>

namespace cursed
{
    struct Tile
    {
        int code;
        int type;
        bool passable;
        TCODColor fg = TCODColor::white;

        Tile() : code( '?' ), type( TT_BLANK ), passable( true ) {}

        Tile( int code )
        {
            this->code = code;
            this->passable = true; // Default
            switch ( code )
            {
                case ' ' :
                    passable=true;
                    break;
                case '#' :
                    passable=false;
                    break;
                default:
                    break;
            }
        }
        Tile( const Tile &tile )
        {
            this->code = tile.code;
            this->passable = tile.passable;
        }
    };
};
