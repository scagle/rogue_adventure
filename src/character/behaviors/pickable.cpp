#include "pickable.hpp"

#include "../actor.hpp"
#include "../../engine.hpp"

namespace cursed
{
    // Static Declaration

    // Constructors

    // Methods
    bool Pickable::pick( Actor &owner, Actor &wearer )
    {
        // See if wearer has inventory, and add item if able
        if ( wearer.inventory 
          && wearer.inventory->moveTo( ITEMS, &owner, &Engine::getArea(), wearer.inventory.get() ) )
        {
            return true;
        }
        return true;
    }

    bool Pickable::use( Actor &owner, Actor &wearer )
    {
        if ( wearer.inventory )
        {
            wearer.inventory->erase( ITEMS, &owner );
            return true;
        }
        return false;
    }

    void Pickable::drop( Actor &owner, Actor &wearer )
    {
        if ( wearer.inventory )
        {
            owner.x = wearer.x;
            owner.y = wearer.y;
            wearer.inventory->moveTo( ITEMS, &owner, wearer.inventory.get(), &Engine::getArea() );
            Engine::getConsole().message( TCODColor::grey, "You dropped %s!", owner.name.c_str() );
        }
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

    bool LightningBolt::use( Actor &owner, Actor &wearer )
    {
        Actor *closest_monster = Engine::getClosestActor( wearer.x, wearer.y, range, false );
        if ( ! closest_monster )
        {
            Engine::getConsole().message(TCODColor::lightGrey,
                "No enemy is close enough to strike.");
            return false;
        }

        // Strike closest monster 
        Engine::getConsole().message( TCODColor::red, 
            "A lightning bolt strikes the %s with a loud thunder!\n"
            "The damage is %g hit points.", closest_monster->name.c_str(), damage );
        closest_monster->destructible->takeDamage( *closest_monster, damage );

        return Pickable::use( owner, wearer );
    }

    bool Fireball::use( Actor &owner, Actor &wearer )
    {
        Engine::getConsole().message( TCODColor::cyan,
            "Left-click to target tile for the fireball,\nor right-click to cancel." );
        int x, y;
        if ( ! Engine::pickATile( &x, &y ) )
        {
            return false;
        }
        Engine::getConsole().message( TCODColor::orange,
            "The fireball explodes, burning everything within %g tiles", range );
        // Burn everything in range including player
        for ( auto&& actor : Engine::getActors() )
        {
            if ( ( actor->destructible && ! actor->destructible->isDead() ) &&
                 ( actor->getDistance( x, y ) <= range ) )
            {
                Engine::getConsole().message( TCODColor::orange,
                    "%s gets burned for  %g hit points", actor->name.c_str(), damage );
                actor->destructible->takeDamage( *actor, damage );
            }
        }

        return Pickable::use( owner, wearer );
    }
    bool Confuser::use( Actor &owner, Actor &wearer )
    {
        Engine::getConsole().message( TCODColor::cyan,
            "Left-click creature to confuse it,\nor right-click to cancel." );
        int x, y;
        if ( ! Engine::pickATile( &x, &y, range ) )
        {
            return false;
        }

        Actor *actor = Engine::getClosestActor( x, y, range, true );
        if ( !actor )
        {
            return false;
        }

        // Confuse monster
        actor->ai = std::make_unique< ConfusedMonsterAI >( 5, std::move( actor->ai ) );
        Engine::getConsole().message( TCODColor::lightBlue,
            "The eyes of %s look vacant,\nas he starts to stumble around!", actor->name.c_str() );

        return Pickable::use( owner, wearer );
    }
};
