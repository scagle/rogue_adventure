#include "attacker.hpp"

#include "../actor.hpp"
#include "../../engine.hpp"

#include <libtcod/libtcod.hpp>

namespace cursed
{
    // Static Declaration

    // Constructors
    Attacker::Attacker( float power ) : power( power )
    {
        
    }

    // Methods
    void Attacker::attack( Actor &owner, Actor &target )
    {
        if ( target.destructible && !target.destructible->isDead() )
        {
            if ( power - target.destructible->defense > 0 )
            {
                Engine::getConsole().message(TCODColor::lightRed, 
                    "%s attacks %s for %g hit points.\n", 
                    owner.name.c_str(), target.name.c_str(), 
                    power - target.destructible->defense
                );
            } else {
                Engine::getConsole().message(TCODColor::lightRed, 
                    "%s attacks %s but it has no effect...\n", 
                    owner.name.c_str(), target.name.c_str()
                );
            }
            target.destructible->takeDamage( target, power );
        } else {
            Engine::getConsole().message(TCODColor::lightRed, "%s attacks %s in vain...\n", 
                owner.name.c_str(), target.name.c_str()
            );
        }
    }
};
