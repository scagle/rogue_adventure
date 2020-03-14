#include "actor.hpp"

#include "../world/map.hpp"
#include "../engine.hpp"

#include "../datatypes/array2d.hpp"

#include <libtcod/libtcod.hpp>
#include <math.h>

namespace rogue_adventure
{
    bool Actor::attemptMove( int dx, int dy, Area &map )
    {
        if ( map.isWalkable( this->x + dx, this->y + dy ) )
        {
            move( dx, dy );
            return true;
        }
        return false;
    }

    void Actor::move( int x, int y )
    {
        this->x += x;
        this->y += y;
    }

    bool Actor::moveOrAttack( int x, int y ) 
    {
        Area& map = engine->getArea();
        if ( map.isWall( x, y ) )
        {
            return false;
        }
        for ( auto&& actor : engine->getActors() )
        {
            if ( actor->x == x && actor->y == y )
            {
                Engine::getConsole().message( TCODColor::lightRed, "%s is unphased!\n", 
                    actor->name.c_str() );
                return false;
            }
        }
        this->x = x;
        this->y = y;
        return true;
    }

    float Actor::getDistance( int cx, int cy ) const
    {
        int dx = x - cx;
        int dy = y - cy;
        return sqrtf( dx*dx + dy*dy );
    }

    void Actor::render( Camera &camera ) const
    {
        camera.setChar( x, y, code, &color, nullptr );
    }

    void Actor::update() 
    {
        if ( ai )
        {
            ai->update( *this );
        }
    }
};
