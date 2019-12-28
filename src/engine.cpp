#include "engine.hpp"

#include "actor.hpp"
#include "map.hpp"

#include "enums/game_status.hpp"
#include "enums/container_type.hpp"
#include "behaviors/ai.hpp"
#include "behaviors/attacker.hpp"
#include "behaviors/destructible.hpp"

#include <algorithm>

namespace cursed
{
    extern Engine engine;

    // Static Declaration
    int Engine::screen_width;
    int Engine::screen_height;
    ResourceHandler Engine::resource_handler;
//  std::vector< Actor* > Engine::current_actors;       // all npcs
//  std::vector< Actor* > Engine::current_items;        // all items
    Map *Engine::current_map;        
    int Engine::map_visibility;
    Engine *Engine::active_engine = nullptr;
    GameStatus Engine::game_state;
    TCOD_key_t Engine::current_key;
    TCOD_mouse_t Engine::current_mouse;
    std::unique_ptr< Console > Engine::console;
    Actor *Engine::player = nullptr;

    // Constructors
    Engine::Engine( int screen_width, int screen_height )
    {
        this->screen_width = screen_width;
        this->screen_height = screen_height;
        this->active_engine = this;
        this->game_state = STARTUP;
        this->map_visibility = 12;

        TCODConsole::initRoot( screen_width, screen_height, "Cursed Adventure", false );
        console = std::make_unique< Console >( screen_width, 20 );

        // Resource management
        resource_handler.loadResources();         // initialize ResourceHandler
        current_map = resource_handler.getMap(0); // load map 0

        // Character Creation
        std::unique_ptr< Actor > unique_player = 
            std::make_unique< Actor >( this, 40, 25, '@', "player", TCODColor::white );
        unique_player->destructible = std::make_unique< PlayerDestructible >( 30, 2, 
            true, "your cadaver" );
        unique_player->attacker = std::make_unique< Attacker >( 5 );
        unique_player->ai = std::make_unique< PlayerAI >();
        unique_player->pickable = nullptr;
        unique_player->inventory = std::make_unique< Inventory >( 26 );
        this->player = unique_player.get();
        current_map->add( CREATURES, std::move( unique_player ) );

        //this->setActors( current_map );           // load all actors

    }

    Engine::~Engine()
    {

    }

    // Methods
    
    // Get references of smart_pointer values, as we don't need to share ownership
//  void Engine::setActors( Map *map )
//      this->current_actors = current_map->getContainer( CREATURES );
//      this->current_items = current_map->getContainer( ITEMS );
//  {
//      // 1. Get NPCs
//      std::vector< std::unique_ptr< Actor > > &actors = current_map->getActors();
//      current_actors.clear();

//      // Copy and add actors to list
//      for ( auto&& actor : actors )
//          current_actors.push_back( actor.get() );


//      // 2. Get Items
//      std::vector< std::unique_ptr< Actor > > &items = current_map->getItems();
//      current_items.clear();

//      // Copy and add items to list
//      for ( auto&& item : items )
//          current_items.push_back( item.get() );
//  }

    void Engine::sendToBack( ContainerType type, Actor &actor )
    {
        current_map->moveToAt( type, &actor, current_map, current_map, 0 );
    }

    bool Engine::pickATile( int *x, int *y, float max_range )
    {
        while ( ! TCODConsole::isWindowClosed() ) 
        {
            // highlight possible range
            for ( int cx = 0; cx < current_map->getWidth(); cx++ )
            {
                for ( int cy = 0; cy < current_map->getHeight(); cy++ )
                {
                    if ( current_map->isInFov( cx, cy ) && 
                        ( max_range == 0 || player->getDistance( cx, cy ) <= max_range ) )
                    {
                        TCODColor color = TCODConsole::root->getCharBackground( cx, cy );
                        color = color * 1.2;
                        TCODConsole::root->setCharBackground( cx, cy, color );
                    }
                    if ( ( current_map->isInFov( current_mouse.cx, current_mouse.cy ) ) && 
                         ( max_range == 0 || 
                           player->getDistance( 
                                current_mouse.cx, current_mouse.cy 
                           ) <= max_range ) )
                    {
                        TCODConsole::root->setCharBackground( current_mouse.cx, current_mouse.cy, 
                            TCODColor::white );
                        if ( current_mouse.lbutton_pressed ) 
                        {
                            *x = current_mouse.cx;
                            *y = current_mouse.cy;
                            return true;
                        }
                    }
                }
            }

            getEngine()->render();
            TCODConsole::flush();

            TCODSystem::waitForEvent( TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE, 
                &current_key, &current_mouse, true );

            if ( current_mouse.rbutton_pressed || current_key.vk != TCODK_NONE )
            {
                return false;
            }
        }
        return false;
    }

    Actor* Engine::getClosestActor( int x, int y, float range,  
        bool player_included )
    {
        Actor *closest_actor = nullptr;
        float best_distance = 1000000; // Ridiculously high distance
        for ( auto&& actor : getActors() )
        {
            // If player_included then always true.
            // Or otherwise, if actor is not a player then set true  
            bool player_inclusion_passing_condition = ( player_included || actor.get() != player );

            if ( ( player_inclusion_passing_condition )
              && ( actor->destructible && ! actor->destructible->isDead() ) )
            {
                float distance = actor->getDistance( x, y );
                if ( distance < best_distance && ( distance <= range || range == 0 ) )
                {
                    best_distance = distance;
                    closest_actor = actor.get();
                    if ( distance == 0 )
                    {
                        return closest_actor; // You're not going to find one closer than 0
                    }
                }
            }
        }
        return closest_actor;
    }

    void Engine::update( bool blocking )
    {
        if ( game_state == STARTUP )
        {
            current_map->computeFov( *player, map_visibility );
        }

        game_state = IDLE;
        if ( blocking )
        {
            TCODSystem::waitForEvent( TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE, 
                &current_key, &current_mouse, true);
        }
        else
        {
            TCODSystem::checkForEvent( TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE, 
                &current_key, &current_mouse );
        }

        // Update Player
        player->update();

        // Update NPCs
        if ( game_state == NEW_TURN )
        {
            for ( auto&& actor : getActors() )
            {
                if ( actor.get() != player )    
                {
                    actor->update();
                }
            }
        }
    }

    void Engine::render()
    {
        TCODConsole::root->clear();

        // Render Map
        current_map->render();

        // Render items
        for ( auto&& actor : current_map->getContainer( ITEMS ) )
        {
            if ( current_map->isInFov( actor->x, actor->y ) )
            {
                actor->render();
            }
        }

        // Render creatures
        for ( auto&& actor : current_map->getContainer( CREATURES ) )
        {
            if ( current_map->isInFov( actor->x, actor->y ) )
            {
                if ( actor.get() != player )
                {
                    actor->render();
                }
            }
        }

        // Render the player
        player->render();

        // Show log and stats
        console->render();

    }
};

