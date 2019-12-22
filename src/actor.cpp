#include "actor.hpp"

#include "map.hpp"
#include "engine.hpp"

#include "datatypes/array2d.hpp"

#include <libtcod/libtcod.hpp>

namespace cursed
{
    bool Actor::attemptMove( int dx, int dy, Map *map )
    {
        if ( map->isWalkable( this->x + dx, this->y + dy ) )
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
        Map* map = engine->getMap();
        if ( map->isWall( x, y ) )
        {
            return false;
        }
        for ( auto *actor : *engine->getActors() )
        {
            if ( actor->x == x && actor->y == y )
            {
                Engine::getConsole()->message( TCODColor::lightRed, "%s is unphased!\n", 
                    actor->name.c_str() );
                return false;
            }
        }
        this->x = x;
        this->y = y;
        return true;
    }

    void Actor::render() const
    {
        TCODConsole::root->setChar(x, y, code);
        TCODConsole::root->setCharForeground(x, y, color);
    }

    void Actor::update() 
    {
        if ( ai )
        {
            ai->update( this );
        }
    }
};
