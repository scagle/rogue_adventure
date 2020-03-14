#include "pressable.hpp"

#include "gui.hpp"

namespace rogue_adventure
{
    // Static Declaration

    // Constructors
    Pressable::Pressable()
    {
        
    }

    // Methods
    void Pressable::update( GUI *owner )
    {

    }

    void Pressable::render( TCODConsole *console, GUI *owner )
    {
        Bound& bound = owner->getBound();
        TCODColor old_color = console->getDefaultBackground();
        console->setDefaultBackground( selected_color );
        console->rect( bound.x, bound.y, bound.w, bound.h, true, TCOD_BKGND_SET );
        console->setDefaultBackground( old_color );
    }
};
