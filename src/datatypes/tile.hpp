#pragma once 

#include <libtcod/libtcod.hpp>

namespace cursed
{
    const TCODColor dirt_wall(128, 80, 60);
    const TCODColor dirt_floor(60, 40, 10);

    enum ColorType
    {
        FG,
        BG
    };

    struct Tile
    {
        int code;
        bool walkable;
        bool transparent;
        bool explored;
        TCODColor fg = TCODColor::white;
        TCODColor bg = TCODColor::black;

        Tile(){ construct(); }
        Tile( int code )
        {
            construct( code=code );
            switch ( code )
            {
                case ' ' :
                    walkable=true;
                    transparent=true;
                    fg = dirt_wall;
                    bg = dirt_floor;
                    break;
                case '#' :
                    walkable=false;
                    transparent=false;
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
            this->walkable = tile.walkable;
            this->transparent = tile.transparent;
            this->fg = tile.fg;
            this->bg = tile.bg;
        }

        void construct( int code='?', 
                        bool walkable=true, 
                        bool transparent=true,
                        bool explored=false )
        {
            this->code = code;    
            this->transparent = transparent;    
            this->walkable = walkable;    
            this->explored = explored;    
        }

        TCODColor getOffsetColor( ColorType type, float offset )
        {
            switch ( type )
            {
                case FG:
                    return fg * offset;
                case BG:
                    return bg * offset;
                default:
                    printf("*** WARNING: Invalid color type, returning black (tile.hpp)\n");
                    return TCODColor::black;
            }
        }
    };
};
