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
    std::vector< Actor* > Engine::current_actors;        
    Map *Engine::current_map;        
    Actor Engine::player;
    int Engine::map_visibility;
    Engine *Engine::active_engine = nullptr;
    GameStatus Engine::game_state;
    TCOD_key_t Engine::current_key;
    TCOD_mouse_t Engine::current_mouse;
    std::shared_ptr< Console > Engine::console;

    // Constructors
    Engine::Engine( int screen_width, int screen_height )
    {
        this->screen_width = screen_width;
        this->screen_height = screen_height;
        this->active_engine = this;
        this->game_state = STARTUP;
        this->map_visibility = 12;
        TCODConsole::initRoot(screen_width, screen_height, "Cursed Adventure", false);

        // Character Creation
        player = Actor( this, 40, 25, '@', "player", TCODColor::white );
        player.destructible = std::make_shared< PlayerDestructible >( 30, 2, true, "your cadaver");
        player.attacker = std::make_shared< Attacker >( 5 );
        player.ai = std::make_shared< PlayerAI >();

        resource_handler.loadResources();
        current_map = resource_handler.getMap(0);
        console = std::make_shared< Console >( screen_width, 8 );
        getAllActors( current_map );
    }

    Engine::~Engine()
    {

    }

    // Methods
    void Engine::sendToBack( Actor *actor )
    {
        current_actors.erase( std::remove( current_actors.begin(), current_actors.end(), actor ), 
            current_actors.end() );
        current_actors.insert( current_actors.begin(), actor );
    }

    void Engine::update()
    {
        if ( game_state == STARTUP )
        {
            current_map->computeFov( &(player), map_visibility );
        }

        game_state = IDLE;
        TCODSystem::checkForEvent( TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE, 
            &current_key, &current_mouse );

        // Update Player
        player.update();

        // Update NPCs
        if ( game_state == NEW_TURN )
        {
            for ( auto *actor : current_actors )
            {
                if ( actor != &player )    
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

        for ( auto *actor : current_actors )
        {
            if ( current_map->isInFov( actor->x, actor->y ) )
            {
                actor->render();
            }
        }

        // Render the player
        player.render();

        // Show log and stats
        console->render();

    }

    void Engine::getAllActors( Map *map )
    {
        std::vector< Actor* > *actors = current_map->getActors();
        current_actors = *actors; // Copy Actors
        current_actors.push_back( &player ); // Add player to list of actors
    }
};

