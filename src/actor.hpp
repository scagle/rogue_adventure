#pragma once
namespace cursed
{
    class Map;
}

#include "datatypes/position.hpp"

#include <libtcod/libtcod.hpp>

namespace cursed
{
    class Actor
    {
        public:
        int x;
        int y;

        protected:
        int code = '?'; 
        TCODColor color; 

        public:
        Actor() : x( 0 ), y( 0 ) {}
        Actor( int x, int y, int code, const TCODColor &color ) 
            : x( x ), y( y ), code( code ), color( color ) {}

        virtual bool attemptMove( int dx, int dy, Map *map );
        virtual void move( int x, int y );
        virtual void render() const;
    };
};
