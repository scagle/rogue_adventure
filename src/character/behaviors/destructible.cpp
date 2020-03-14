#include "destructible.hpp"

#include "../actor.hpp"
#include "../../engine.hpp"
#include "../../enums/game_status.hpp"

namespace rogue_adventure
{
    // Static Declaration

    // Constructors
    Destructible::Destructible( float max_hp, float defense, bool corpse_visible, 
        std::string corpse_name ) : 
        max_hp( max_hp ), hp( max_hp ), defense( defense ), corpse_visible( corpse_visible ),
        corpse_name( corpse_name ) 
    {

    }

    MonsterDestructible::MonsterDestructible( float max_hp, float defense, bool corpse_visible, 
        std::string corpse_name ) : 
        Destructible( max_hp, defense, corpse_visible, corpse_name) 
    { 

    }

    PlayerDestructible::PlayerDestructible( float max_hp, float defense, bool corpse_visible, 
        std::string corpse_name ) : 
        Destructible( max_hp, defense, corpse_visible, corpse_name) 
    { 

    }

    // Methods
    float Destructible::takeDamage( Actor &owner, float damage )
    {
        damage -= defense;
        if ( damage > 0 )
        {
            hp -= damage;
            if ( hp <= 0 )
            {
                die( owner );
            }
        } else {
            damage = 0;
        }
        return damage;
    }

    float Destructible::heal( float amount )
    {
        hp += amount;
        if ( hp > max_hp )
        {
            amount -= hp - max_hp;
            hp = max_hp;
        }
        return amount;
    }

    void Destructible::die( Actor &owner )
    {
        // Generate a corpse
        owner.code = '%';
        owner.color = TCODColor::darkRed;
        owner.name = corpse_name;
        owner.blocks = false;
        Engine::sendToBack( CREATURES, owner); // Move body so it appears below other alive actors
    }

    void MonsterDestructible::die( Actor &owner )
    {
        // Spawn corpse
        Engine::getConsole().message(TCODColor::red, "%s is dead!\n", owner.name.c_str());
        Destructible::die( owner );
    }

    void PlayerDestructible::die( Actor &owner )
    {
        // Spawn corpse
        Engine::getConsole().message(TCODColor::red, "You died!\n", owner.name.c_str());
        Destructible::die( owner );
        Engine::setState( DEFEAT );
    }
};
