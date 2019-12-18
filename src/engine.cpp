#include "engine.hpp"
#include "actor.hpp"
#include "map.hpp"

#include "enums/game_status.hpp"

namespace cursed
{
    extern Engine engine;

    // Static Declaration
    ResourceHandler Engine::resource_handler;
    std::vector< Actor* > Engine::current_actors;        
    Map* Engine::current_map;        
    Actor Engine::player;
    int Engine::map_visibility;
    Engine* Engine::active_engine = nullptr;
    GameStatus Engine::game_state;

    // Constructors
    Engine::Engine()
    {
        this->active_engine = this;
        this->game_state = STARTUP;
        TCODConsole::initRoot(80, 50, "Cursed Adventure", false);

        resource_handler.loadResources();
        current_map = resource_handler.getMap(0);
        getActorsInMap( current_map );
        player = Actor( this, 40, 25, '@', "player", TCODColor::white );
        map_visibility = 12;
    }

    // Methods
    void Engine::getActorsInMap( Map *map )
    {
        std::vector< Actor* > *actors = current_map->getActors();
        current_actors = *actors; // Copy Actors
        current_actors.push_back( &player ); // Add player to list of actors
    }

    void Engine::update()
    {
        if ( game_state == STARTUP )
        {
            current_map->computeFov( player, map_visibility );
        }

        game_state = IDLE;
        TCOD_key_t key;
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

        // Update Player
        int dx = 0;
        int dy = 0;
        switch ( key.vk )
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
        if ( dx != 0 || dy != 0 )
        {
            game_state = NEW_TURN;
            if ( player.moveOrAttack( player.x + dx, player.y + dy ) )
            {
                current_map->computeFov( player, map_visibility );
            }
        }

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
        player.render();

    }
};

