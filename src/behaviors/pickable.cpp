#include "pickable.hpp"

#include "../actor.hpp"
#include "../engine.hpp"

namespace cursed
{
    // Static Declaration

    // Constructors
    Pickable::Pickable() { }
    Healer::Healer( float amount ) : amount( amount ) { }

    // Methods
    bool Pickable::pick( Actor &owner, Actor &wearer )
    {
        // See if wearer has container, and add item if able
        if ( wearer.container && wearer.container->add( &owner ) )
        {
            // Also remove from list of actors in map
            auto& actors = Engine::getAllActors();
            actors.erase( std::remove( actors.begin(), actors.end(), &owner ), actors.end() );
            return true;
        }
        return true;
    }

    bool Pickable::use( Actor &owner, Actor &wearer )
    {
        if ( wearer.container )
        {
            wearer.container->remove( &owner );
            return true;
        }
        return false;
    }

    bool Healer::use( Actor &owner, Actor &wearer )
    {
        if ( wearer.destructible )
        {
            float amount_healed = wearer.destructible->heal( amount );
            if ( amount_healed > 0 )
            {
                Engine::getConsole().message( TCODColor::green, "You use %s, and heal for %f hp!", 
                    owner.name.c_str(), amount_healed );
                return Pickable::use( owner, wearer );
            }
            else
            {
                Engine::getConsole().message( TCODColor::grey, "You're already at full hp!" );
                return false;
            }
        }
        return false;
    }
};
