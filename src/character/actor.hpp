#pragma once
namespace cursed
{
    class Area;
    class Engine;
    class Camera;
}

#include "../datatypes/position.hpp"

#include "behaviors/ai.hpp"
#include "behaviors/attacker.hpp"
#include "behaviors/destructible.hpp"
#include "behaviors/pickable.hpp"
#include "behaviors/inventory.hpp"

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

        // Composite Elements
        std::unique_ptr< Destructible > destructible;
        std::unique_ptr< Attacker     > attacker;
        std::unique_ptr< AI           > ai;
        std::unique_ptr< Pickable     > pickable;
        std::unique_ptr< Inventory    > inventory;

        Actor() : x( 0 ), y( 0 ) {}
        Actor( Engine *engine, int x, int y, int code, std::string name, const TCODColor &color ) :
            engine( engine ), x( x ), y( y ), code( code ), name( name ), color( color ) {}
        virtual ~Actor() { }

        virtual bool moveOrAttack( int x, int y );
        virtual bool attemptMove( int dx, int dy, Area &map );
        virtual void move( int x, int y );
        virtual float getDistance( int cx, int cy ) const; 
        virtual void render( Camera &camera ) const;
        virtual void update();
    };
};
