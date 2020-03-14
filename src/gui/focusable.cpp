#include "focusable.hpp"

#include "gui.hpp"

namespace rogue_adventure
{
    // Static Declaration

    // Constructors
    Focusable::Focusable()
    {
        
    }

    // Methods
    void Focusable::update( GUI *owner )
    {

    }

    void Focusable::render( TCODConsole *console, GUI *owner )
    {
        Bound &bound = owner->getBound();
        TCODColor old_color = console->getDefaultBackground();
        console->setDefaultBackground( getFocusColor() );
        console->rect( bound.x, bound.y, 3, 3, true, TCOD_BKGND_SET );
        console->rect( bound.x + bound.w - 3, bound.y, 3, 3, true, TCOD_BKGND_SET );
        console->printRectEx( bound.x+1, bound.y+1, 1, 1, 
            TCOD_BKGND_NONE, TCOD_CENTER, "-" );
        console->printRectEx( bound.x + bound.w - 2, bound.y+1, 1, 1, 
            TCOD_BKGND_NONE, TCOD_CENTER, "+" );
        console->setDefaultBackground( old_color );
    }
};
