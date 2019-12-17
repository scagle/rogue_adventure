#include "actor.hpp"

#include "map.hpp"

#include "datatypes/array2d.hpp"

#include <libtcod/libtcod.hpp>

namespace cursed
{
    bool Actor::attemptMove( int dx, int dy, Map *map )
    {
        if ( map->isPassable( this->x + dx, this->y + dy ) )
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

    void Actor::render() const
    {
        TCODConsole::root->setChar(x, y, code);
        TCODConsole::root->setCharForeground(x, y, color);
    }
};
