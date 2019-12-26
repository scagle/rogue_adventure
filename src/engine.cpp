#include "engine.hpp"

#include "actor.hpp"
#include "map.hpp"

#include "enums/game_status.hpp"
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
    std::vector< Actor* > Engine::current_actors;       // all npcs
    std::vector< Actor* > Engine::all_current_actors;   // all npcs + player                                                    
    std::vector< Actor* > Engine::current_items;        // all items
    Map *Engine::current_map;        
    int Engine::map_visibility;
    Engine *Engine::active_engine = nullptr;
    GameStatus Engine::game_state;
    TCOD_key_t Engine::current_key;
    TCOD_mouse_t Engine::current_mouse;
    std::unique_ptr< Console > Engine::console;
    std::unique_ptr< Actor > Engine::player;

    // Constructors
    Engine::Engine( int screen_width, int screen_height )
    {
        this->screen_width = screen_width;
        this->screen_height = screen_height;
        this->active_engine = this;
        this->game_state = STARTUP;
        this->map_visibility = 12;

        TCODConsole::initRoot( screen_width, screen_height, "Cursed Adventure", false );
        console = std::make_unique< Console >( screen_width, 8 );

        // Character Creation
        player = std::make_unique< Actor >( this, 40, 25, '@', "player", TCODColor::white );
        player->destructible = std::make_unique< PlayerDestructible >( 30, 2, 
            true, "your cadaver" );
        player->attacker = std::make_unique< Attacker >( 5 );
        player->ai = std::make_unique< PlayerAI >();
        player->pickable = nullptr;
        player->container = std::make_unique< Container >( 26 );

        // Resource management
        resource_handler.loadResources();         // initialize ResourceHandler
        current_map = resource_handler.getMap(0); // load map 0
        this->setActors( current_map );           // load actors
    }

    Engine::~Engine()
    {

    }

    // Methods
    void Engine::sendToBack( Actor &actor )
    {
        current_actors.erase( std::remove( current_actors.begin(), current_actors.end(), &actor ), 
            current_actors.end() );
        current_actors.insert( current_actors.begin(), &actor );
    }

    void Engine::eraseActor( Actor *target )
    {
        for ( auto&& actor : current_actors )
        {
            if ( target == actor )
            {
                current_actors.erase( std::remove( current_actors.begin(), current_actors.end(), 
                    actor ), current_actors.end() );
                return;
            }
        }

        for ( auto&& actor : current_items )
        {
            if ( target == actor )
            {
                current_items.erase( std::remove( current_items.begin(), current_items.end(), 
                    actor ), current_items.end() );
                return;
            }
        }
    }

    void Engine::update()
    {
        if ( game_state == STARTUP )
        {
            current_map->computeFov( *player, map_visibility );
        }

        game_state = IDLE;
        TCODSystem::checkForEvent( TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE, 
            &current_key, &current_mouse );

        // Update Player
        player->update();

        // Update NPCs
        if ( game_state == NEW_TURN )
        {
            for ( auto&& actor : current_actors )
            {
                if ( actor != player.get() )    
                {
                    actor->update();
                }
            }
        }
    }

    void Engine::render()
    {
        TCODConsole::root->clear();

        // Rendering
        current_map->render();

        for ( auto&& actor : current_items )
        {
            if ( current_map->isInFov( actor->x, actor->y ) )
            {
                actor->render();
            }
        }

        for ( auto&& actor : current_actors )
        {
            if ( current_map->isInFov( actor->x, actor->y ) )
            {
                actor->render();
            }
        }

        // Render the player
        player->render();

        // Show log and stats
        console->render();

    }

    void Engine::setActors( Map *map )
    {
        // 1. Get NPCs
        std::vector< std::unique_ptr< Actor > > &actors = current_map->getActors();
        current_actors.clear();

        // Copy and add actors to list
        for ( auto&& actor : actors )
            current_actors.push_back( actor.get() );


        // 2. Get Items
        std::vector< std::unique_ptr< Actor > > &items = current_map->getItems();
        current_items.clear();

        // Copy and add items to list
        for ( auto&& item : items )
            current_items.push_back( item.get() );

        // 3. Get All Actors, including the main player
        all_current_actors.clear();
        all_current_actors.insert( all_current_actors.end(),  // Concatenate npcs
            current_actors.begin(), current_actors.end() );
        all_current_actors.insert( all_current_actors.end(),  // Concatenate items
            current_items.begin(), current_items.end() );
        all_current_actors.push_back( player.get() );         // Concatenate player
    }
};

