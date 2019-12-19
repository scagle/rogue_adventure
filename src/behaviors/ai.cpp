#include "ai.hpp"

#include "../actor.hpp"
#include "../engine.hpp"

#include <libtcod/libtcod.hpp>
#include <math.h>

namespace cursed
{
    // Static Declaration

    // Constructors
    AI::AI()
    {
        
    }

    // Methods
    void PlayerAI::update( Actor *owner )
    {
        // Check to see if dead
        if ( owner->destructible && owner->destructible->isDead() )
        {
            return;
        }

        // Update Player
        int dx = 0;
        int dy = 0;
        switch ( Engine::getCurrentKey()->vk )
        {
            case TCODK_UP: 
                dx = 0; dy = -1;
                break;
            case TCODK_RIGHT: 
                dx = 1; dy = 0;
                break;
            case TCODK_LEFT: 
                dx = -1; dy = 0;
                break;
            case TCODK_DOWN: 
                dx = 0; dy = 1;
                break;
            default: break;
        }

        // Update Player
        Engine *engine = Engine::getEngine();
        Map *map = engine->getMap();
        if ( dx != 0 || dy != 0 )
        {
            engine->setState( NEW_TURN );
            if ( moveOrAttack( owner, owner->x + dx, owner->y + dy ) )
            {
                map->computeFov( owner, Engine::getVisibility() );
            }
        }
    }

    bool PlayerAI::moveOrAttack( Actor *owner, int target_x, int target_y )
    {
        Engine *engine = Engine::getEngine();
        if ( engine->getMap()->isWall( target_x, target_y ) )
        {
            return false;
        }
        for ( auto *actor : *(engine->getActors()) )
        {
            // check npcs
            if ( ( actor->destructible ) && ( !actor->destructible->isDead() ) &&
                 ( actor->x == target_x ) && ( actor->y == target_y ) )
            {
                owner->attacker->attack( owner, actor );
                return false;
            }
            // check corpses
            else if ( ( actor->destructible ) && ( actor->destructible->isDead() ) &&
                      ( actor->x == target_x ) && ( actor->y == target_y ) )
            {
                printf("Theres a %s here!\n", actor->name.c_str()); 
            }
        }
        owner->x = target_x;
        owner->y = target_y;
        return true;
    }

    void MonsterAI::update( Actor *owner )
    {
        // Check to see if dead
        if ( owner->destructible && owner->destructible->isDead() )
        {
            return;
        }
        Engine *engine = Engine::getEngine();
        // Check to see if monster is in field of view of player
        if ( engine->getMap()->isInFov( owner->x, owner->y ) )
        {
            moveOrAttack( owner, engine->getPlayer()->x, engine->getPlayer()->y );
        }
    }

    bool MonsterAI::moveOrAttack( Actor *owner, int target_x, int target_y )
    {
        Engine *engine = Engine::getEngine();
        Map *map = engine->getMap();
        int dx = target_x - owner->x;
        int dy = target_y - owner->y;
        float distance = sqrtf(dx*dx + dy*dy);

        if ( distance >= 2 )
        {
            dx = (int)( round( dx / distance ) );
            dy = (int)( round( dy / distance ) );
            if ( map->isWalkable( owner->x + dx, owner->y + dy ) )
            {
                owner->x += dx;
                owner->y += dy;
                return true;
            }
        } 
        else if ( owner->attacker )
        {
            owner->attacker->attack( owner, engine->getPlayer() );
        }
        return false;
    }
};
