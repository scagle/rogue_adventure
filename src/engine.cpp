#include "engine.hpp"
#include "actor.hpp"
#include "map.hpp"

namespace cursed
{
    // Static Declaration

    // Constructors
    Engine::Engine()
    {
        TCODConsole::initRoot(80, 50, "Cursed Adventure", false);

        resource_handler.loadResources();
        current_map = resource_handler.getMap(0);
        current_actors = current_map->getActors();
        player = Actor(40, 25, '@', TCODColor::white);
    }

    // Methods
    void Engine::update()
    {
        TCOD_key_t key;
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

        // Update Player
        switch (key.vk)
        {
            case TCODK_UP: 
                player.attemptMove(  0, -1, current_map );
                break;
            case TCODK_RIGHT: 
                player.attemptMove(  1,  0, current_map );  
                break;
            case TCODK_LEFT: 
                player.attemptMove( -1,  0, current_map ); 
                break;
            case TCODK_DOWN: 
                player.attemptMove(  0,  1, current_map );  
                break;
            default: break;
        }
    }

    void Engine::render()
    {
        TCODConsole::root->clear();

        // Rendering
        current_map->render();

        for ( auto actor : *current_actors )
        {
            actor->render();
        }
        player.render();

    }
};
