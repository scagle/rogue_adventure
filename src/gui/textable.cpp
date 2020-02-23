#include "textable.hpp"

#include "gui.hpp"

namespace cursed
{
    // Static Declaration

    // Constructors
    Textable::Textable()
    {
        
    }

    // Methods
    void Textable::update( GUI *owner )
    {
        
    }
    
    void Textable::render( TCODConsole *console, GUI *owner )
    {
        Bound &bound = owner->getBound();
        Bound cbound = bound.getCentered();
        console->printRectEx( cbound.x, cbound.y, cbound.w, cbound.h, 
            TCOD_BKGND_NONE, TCOD_CENTER, getText().c_str() );
    }
};
