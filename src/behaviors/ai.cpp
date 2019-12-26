#include "ai.hpp"

#include "../actor.hpp"
#include "../engine.hpp"
#include "../options.hpp"

#include "../enums/game_status.hpp"

#include <libtcod/libtcod.hpp>
#include <math.h>

namespace cursed
{
    // Static Declaration
    // How many turns it will track after losing sight
    const int MONSTER_TRACKING_TURNS = 3; 
    TCODConsole PlayerAI::inv_console( INVENTORY_WIDTH, INVENTORY_HEIGHT );

    // Constructors
    AI::AI()
    {
        
    }

    // Methods
    bool AI::pickUpItem( Actor& owner )
    {
        bool found = false;
        bool picked_up = false;
        for ( auto&& item : Engine::getItems() )
        {
            if ( item->pickable && item->x == owner.x && item->y == owner.y )
            {

                found = true;
                if ( item->pickable->pick( *item, owner ) )
                {
                    Engine::getConsole().message( TCODColor::lightGrey, "You pick up the %s.",
                        item->name.c_str() );
                    picked_up = true;
                    //break;
                }
                else if ( !found )
                {
                    Engine::getConsole().message( TCODColor::lightGrey, "Your inventory is full!" );
                }
                // NOTE: will stop when it see's the first item it can't break up. 
                //       Make sure to change this when you implement "weight" mechanics in future.
                break;  
            }
        }
        if ( !found )
        {
            Engine::getConsole().message(TCODColor::lightGrey, "There's nothing here!" );
        }
        Engine::setState( NEW_TURN );
        return picked_up;
    }

    void PlayerAI::update( Actor &owner )
    {
        // Check to see if dead
        if ( owner.destructible && owner.destructible->isDead() )
        {
            return;
        }

        // Update Player
        int dx = 0;
        int dy = 0;
        int key = Engine::getCurrentKey().c;
        Options &options = Options::getOptions();
        if ( key == options.UP )
        {
            dx = 0; dy = -1;
        }
        else if ( key == options.DOWN )
        {
            dx = 0; dy = 1;
        }
        else if ( key == options.LEFT )
        {
            dx = -1; dy = 0;
        }
        else if ( key == options.RIGHT )
        {
            dx = 1; dy = 0;
        }
        else if ( key == options.PICK_UP )
        {
            AI::pickUpItem( owner );
        }
        else if ( key == options.INVENTORY )
        {
            Actor *actor = chooseFromInventory( owner );
            if ( actor )
            {
                actor->pickable->use( *actor, owner );
                Engine::setState( NEW_TURN );
            }
        }

        // Update Player
        Engine *engine = Engine::getEngine();
        Map &map = engine->getMap();
        if ( dx != 0 || dy != 0 )
        {
            engine->setState( NEW_TURN );
            if ( moveOrAttack( owner, owner.x + dx, owner.y + dy ) )
            {
                map.computeFov( owner, Engine::getVisibility() );
            }
        }
    }

    bool PlayerAI::moveOrAttack( Actor &owner, int target_x, int target_y )
    {
        Engine *engine = Engine::getEngine();
        if ( engine->getMap().isWall( target_x, target_y ) )
        {
            return false;
        }
        for ( auto&& actor : engine->getAllActors() )
        {
            bool is_corpse_or_item = ( actor->destructible && actor->destructible->isDead() )
                || ( actor->pickable ) ;
            // check npcs
            if ( ( actor->destructible ) && ( !actor->destructible->isDead() ) &&
                 ( actor->x == target_x ) && ( actor->y == target_y ) )
            {
                owner.attacker->attack( owner, *actor );
                return false;
            }
            // check corpses or items
            else if ( ( is_corpse_or_item ) &&
                      ( actor->x == target_x ) && ( actor->y == target_y ) )
            {
                Engine::getConsole().message(TCODColor::yellow, "Theres a %s here!\n", 
                    actor->name.c_str()); 
            }
        }
        owner.x = target_x;
        owner.y = target_y;
        return true;
    }

    // Can return nullptr if no item is found
    Actor* PlayerAI::chooseFromInventory( Actor &owner ) 
    {
        inv_console.setDefaultForeground( TCODColor(200, 180, 50 ) );
        inv_console.printFrame( 0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, 
            "Inventory");

        inv_console.setDefaultForeground( TCODColor::white );

        std::vector< Actor* >& inventory = owner.container->getContainer();
        int shortcut = 'a'; // Key shortcut to press for each item
        int y_offset = 1;   // Display downwards
        for ( auto&& actor : inventory )
        {
            inv_console.printf( 2, y_offset, "(%c) %s", shortcut, actor->name.c_str() );
            y_offset++;
            shortcut++;
        }

        // blit the inventory console onto the root console
        int screen_width = Engine::getEngine()->getScreenWidth();
        int screen_height = Engine::getEngine()->getScreenHeight();
        TCODConsole::blit( &inv_console, 0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT,
            TCODConsole::root, screen_width / 2 - INVENTORY_WIDTH / 2,
            screen_height / 2 - INVENTORY_HEIGHT / 2 );
        TCODConsole::flush();

        // Wait for a key press
        TCOD_key_t key;
        TCODSystem::waitForEvent( TCOD_EVENT_KEY_PRESS, &key, NULL, true );

        if ( key.vk == TCODK_CHAR )
        {
            int actor_index = key.c - 'a';
            if ( actor_index >= 0 && actor_index < owner.container->getContainer().size() )
            {
                return owner.container->getContainer()[actor_index];
            }
        }
        return nullptr;
    }

    void MonsterAI::update( Actor &owner )
    {
        // Check to see if dead
        if ( owner.destructible && owner.destructible->isDead() )
        {
            return;
        }
        Engine *engine = Engine::getEngine();
        // Check to see if monster is in field of view of player
        if ( engine->getMap().isInFov( owner.x, owner.y ) )
        {
            move_count = MONSTER_TRACKING_TURNS;
        } 
        else 
        { 
            move_count--;
        }
        if ( move_count > 0 )
        {
            moveOrAttack( owner, engine->getPlayer().x, engine->getPlayer().y );
        }
    }

    bool MonsterAI::moveOrAttack( Actor &owner, int target_x, int target_y )
    {
        Engine *engine = Engine::getEngine();
        Map &map = engine->getMap();
        int dx = target_x - owner.x;
        int dy = target_y - owner.y;
        int step_dx = (dx > 0) ? 1 : -1;
        int step_dy = (dy > 0) ? 1 : -1;
        float distance = sqrtf(dx*dx + dy*dy);

        if ( distance >= 2 )
        {
            dx = (int)( round( dx / distance ) );
            dy = (int)( round( dy / distance ) );
            if ( map.isWalkable( owner.x + dx, owner.y + dy ) )
            {
                owner.x += dx;
                owner.y += dy;
                return true;
            }
            else if ( map.isWalkable( owner.x + step_dx, owner.y ) )
            {
                owner.x += step_dx;
            }
            else if ( map.isWalkable( owner.x, owner.y + step_dy ) )
            {
                owner.y += step_dy;
            }
        } 
        else if ( owner.attacker )
        {
            owner.attacker->attack( owner, engine->getPlayer() );
        }
        return false;
    }
};
