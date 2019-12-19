#pragma once
namespace cursed
{
    class Map;
    class Engine;
    // Behaviors
    //class Destructible;
    //class Attacker;
    //class AI;
}

#include "datatypes/position.hpp"

#include "behaviors/ai.hpp"
#include "behaviors/attacker.hpp"
#include "behaviors/destructible.hpp"

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
        int code = '?'; 
        std::string name = "Unknown"; 
        TCODColor color = TCODColor::white; 
        bool blocks = true; // Whether character blocks other actor movements
        Destructible *destructible = nullptr;
        Attacker *attacker = nullptr;
        AI *ai = nullptr;

        Actor() : x( 0 ), y( 0 ) {}
        Actor( Engine *engine, int x, int y, int code, std::string name, const TCODColor &color ) :
            engine( engine ), x( x ), y( y ), code( code ), name( name ), color( color ) {}

        virtual bool moveOrAttack( int x, int y );
        virtual bool attemptMove( int dx, int dy, Map *map );
        virtual void move( int x, int y );
        virtual void render() const;
        virtual void update();
    };
};
