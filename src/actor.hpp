#pragma once
namespace cursed
{
    class Map;
    class Engine;
}

#include "datatypes/position.hpp"

#include <string>
#include <libtcod/libtcod.hpp>

namespace cursed
{
    class Actor
    {
        protected:
        Engine *engine = nullptr;

        public:
        int x;
        int y;

        protected:
        int code = '?'; 
        std::string name = "Unknown"; 
        TCODColor color = TCODColor::white; 

        public:
        Actor() : x( 0 ), y( 0 ) {}
        Actor( Engine *engine, int x, int y, int code, std::string name, const TCODColor &color ) 
            : engine( engine ), x( x ), y( y ), code( code ), name( name ), color( color ) {}

        virtual bool moveOrAttack( int x, int y );
        virtual bool attemptMove( int dx, int dy, Map *map );
        virtual void move( int x, int y );
        virtual void render() const;
        virtual void update();
    };
};
