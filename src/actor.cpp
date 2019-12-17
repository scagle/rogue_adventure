#include "actor.hpp"

#include "datatypes/array2d.hpp"

#include <libtcod/libtcod.hpp>

namespace cursed
{
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
