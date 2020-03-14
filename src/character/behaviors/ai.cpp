#include "ai.hpp"

#include "../actor.hpp"
#include "../../engine.hpp"
#include "../../options.hpp"

#include "../../enums/game_status.hpp"
#include "../../enums/entry_point.hpp"
#include "../../world/world.hpp"

#include <libtcod/libtcod.hpp>
#include <math.h>

namespace rogue_adventure
{
    // Static Declaration
    // How many turns it will track after losing sight
    const int MONSTER_TRACKING_TURNS = 3; 
    TCODConsole PlayerAI::inv_console( INVENTORY_WIDTH, INVENTORY_HEIGHT );

    // Constructors
    ConfusedMonsterAI::ConfusedMonsterAI( int turns, std::unique_ptr< AI > old_ai ) 
        : turns( turns )
    {
        this->old_ai = std::move( old_ai ); // Move ownership of old ai to confused ai
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
                std::string name = item->name;
                if ( item->pickable->pick( *item, owner ) )
                {
                    Engine::getConsole().message( TCODColor::lightGrey, "You pick up the %s.",
                        name.c_str() );
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
        static int current_x = 0; // TODO: Temporary variable for testing map switching
        static int current_y = 0; // TODO: Temporary variable for testing map switching
        static bool state = true; // TODO: In Dungeon or Surface state
        // Check to see if dead
        if ( owner.destructible && owner.destructible->isDead() )
        {
            return;
        }

        // Update Player
        int dx = 0;
        int dy = 0;
        TCOD_keycode_t special_key = Engine::getCurrentKey().vk;
        int regular_key = Engine::getCurrentKey().c;
        Options &options = Options::getOptions();

        // Regular Keys
        if ( regular_key == options.UP )
        {
            dx = 0; dy = -1;
        }
        else if ( regular_key == options.DOWN )
        {
            dx = 0; dy = 1;
        }
        else if ( regular_key == options.LEFT )
        {
            dx = -1; dy = 0;
        }
        else if ( regular_key == options.RIGHT )
        {
            dx = 1; dy = 0;
        }
        else if ( regular_key == options.PICK_UP )
        {
            AI::pickUpItem( owner );
        }
        else if ( regular_key == options.INVENTORY )
        {
            Actor *actor = chooseFromInventory( owner );
            if ( actor )
            {
                actor->pickable->use( *actor, owner );
                Engine::setState( NEW_TURN );
            }
        }
        else if ( regular_key == options.DROP )
        {
            Actor *actor = chooseFromInventory( owner );
            if ( actor )
            {
                actor->pickable->drop( *actor, owner );
                Engine::setState( NEW_TURN );
            }
        }
        else if ( regular_key == options.MAP_UP )
        {
            current_y--;
            if ( !Engine::getEngine()->changeMap( state, current_x, current_y ) )
                current_y++;
            Engine::setState( NEW_TURN );
        }
        else if ( regular_key == options.MAP_DOWN )
        {
            current_y++;
            if ( !Engine::getEngine()->changeMap( state, current_x, current_y ) )
                current_y--;
            Engine::setState( NEW_TURN );
        }
        else if ( regular_key == options.MAP_LEFT )
        {
            current_x--;
            if ( !Engine::getEngine()->changeMap( state, current_x, current_y ) )
                current_x++;
            Engine::setState( NEW_TURN );
        }
        else if ( regular_key == options.MAP_RIGHT )
        {
            current_x++;
            if ( !Engine::getEngine()->changeMap( state, current_x, current_y ) )
                current_x--;
            Engine::setState( NEW_TURN );
        }
        else if ( regular_key == options.MAP_SWITCH )
        {
            state ^= 1;
            Engine::getEngine()->changeMap( state, current_x, current_y );
            Engine::setState( NEW_TURN );
        }
        
        // Update Player
        Engine *engine = Engine::getEngine();
        Area &map = engine->getArea();
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
        if ( engine->getArea().isWall( target_x, target_y ) )
        {
            return false;
        }
        // check for npcs
        for ( auto&& actor : engine->getActors() )
        {
            if ( ( actor->destructible ) && ( actor.get() != &owner ) &&
                 ( actor->x == target_x ) && ( actor->y == target_y ) )
            {
                if ( !actor->destructible->isDead() )
                {
                    owner.attacker->attack( owner, *actor );
                    return false;
                }
                else
                {
                    Engine::getConsole().message(TCODColor::yellow, "Theres a %s here!\n", 
                        actor->name.c_str()); 
                }
            }
        }
        for ( auto&& actor : engine->getItems() )
        {
            // check items
            if ( ( actor->x == target_x ) && ( actor->y == target_y ) )
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

        std::vector< std::unique_ptr< Actor > >& inventory = owner.inventory->getContainer( ITEMS );
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
            if ( actor_index >= 0 && actor_index < inventory.size() )
            {
                return inventory[actor_index].get();
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
        if ( engine->getArea().isInFov( owner.x, owner.y ) )
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
        else
        {
            // Aimlessly wander
            TCODRandom *rng = TCODRandom::getInstance();
            int dx = rng->getInt( -1 , 1 );
            int dy = rng->getInt( -1 , 1 );
            int destx = owner.x + dx;
            int desty = owner.y + dy;
            if ( Engine::getArea().isWalkable( destx, desty ) )
            {
                owner.x = destx;
                owner.y = desty;
            }
        }
    }

    bool MonsterAI::moveOrAttack( Actor &owner, int target_x, int target_y )
    {
        Engine *engine = Engine::getEngine();
        Area &map = engine->getArea();
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

    void ConfusedMonsterAI::update( Actor &owner )
    {
        TCODRandom *rng = TCODRandom::getInstance();
        int dx = rng->getInt( -1 , 1 );
        int dy = rng->getInt( -1 , 1 );
        int destx = owner.x + dx;
        int desty = owner.y + dy;
        if ( Engine::getArea().isWalkable( destx, desty ) )
        {
            owner.x = destx;
            owner.y = desty;
        }
        else
        {
            Actor *actor = Engine::getClosestActor( destx, desty, 0, true );
            if ( actor )
            {
                owner.attacker->attack( owner, *actor );
            }
        }
        turns--;
        if ( turns == 0 )
        {
            restore( owner );
        }
    }

    void ConfusedMonsterAI::restore( Actor &owner )
    {
        // Restore old AI back into owner, and let this ai deconstruct itself
        owner.ai = std::move( old_ai );
    }
};
