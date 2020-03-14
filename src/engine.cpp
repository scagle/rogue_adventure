#include "engine.hpp"

#include "camera.hpp"
#include "options.hpp"

#include "enums/game_status.hpp"
#include "enums/container_type.hpp"
#include "enums/menu_type.hpp"

#include "datatypes/mouse.hpp"

#include "character/actor.hpp"
#include "character/behaviors/ai.hpp"
#include "character/behaviors/attacker.hpp"
#include "character/behaviors/destructible.hpp"

#include "world/map.hpp"
#include "world/world.hpp"

#include <algorithm>

namespace rogue_adventure
{
    extern Engine engine;

    // Static Declaration
    bool Engine::close_game = false; // Close game flag
    int Engine::screen_width;
    int Engine::screen_height;
//  ResourceHandler Engine::resource_handler;
//  std::vector< Actor* > Engine::current_actors;       // all npcs
//  std::vector< Actor* > Engine::current_items;        // all items
    World Engine::world;   // World which contains all the maps/zones
    Camera Engine::camera; // Player's camera viewport
    Area *Engine::current_area;        
    int Engine::map_visibility;
    Engine *Engine::active_engine = nullptr;
    GameStatus Engine::game_state; // Current game state 
    GameStatus Engine::temp_state; // Temp storage state
    TCOD_key_t Engine::current_key;
    TCOD_mouse_t Engine::current_mouse;
    std::unique_ptr< Console > Engine::console;
    std::unique_ptr< AgreeMenu > Engine::agree_menu;
    std::unique_ptr< MainMenu > Engine::main_menu;
    std::unique_ptr< GameMenu > Engine::game_menu;
    Actor *Engine::player = nullptr;

    // Constructors
    Engine::Engine( int screen_width, int screen_height )
    {
        this->screen_width = screen_width;
        this->screen_height = screen_height;
        this->active_engine = this;
        this->game_state = STARTUP;
        this->map_visibility = 12;

        TCODConsole::initRoot( screen_width, screen_height, "Rogue Adventure", false );
        init();

    }

    Engine::~Engine()
    {

    }

    // Methods
    void Engine::init()
    {
        // Create console
        console = std::make_unique< Console >( screen_width, 20 );
        
        // Initialize Menus
        initMenus(); 

        // Start out in MainMenu
        spawnMainMenu();

        // World Creation
        world.createWorld( 0x7FFFFFFF );
        current_area = world.getArea();

        // Resource management
//      resource_handler.loadResources();         // initialize ResourceHandler
//      current_area = resource_handler.getArea(0); // load map 0

        // Character Creation
        createMainCharacter("player");

        // Create Player Camera 
        camera.loadCamera( 80, 60, player ); // Load camera / follow player


    }

    void Engine::initMenus()
    {
        // Create Main Menu
        agree_menu = std::make_unique< AgreeMenu >( screen_width, screen_height );

        // Create Main Menu
        main_menu = std::make_unique< MainMenu >( screen_width, screen_height );

        // Create Game Menu
        game_menu = std::make_unique< GameMenu >( screen_width, screen_height );
    }

    void Engine::load()
    {

    }

    void Engine::save()
    {

    }

    void Engine::createMainCharacter( std::string name )
    {
        std::unique_ptr< Actor > unique_player = 
            std::make_unique< Actor >( this, 40, 25, '@', name.c_str(), TCODColor::white );
        unique_player->destructible = std::make_unique< PlayerDestructible >( 30, 2, 
            true, "your cadaver" );
        unique_player->attacker = std::make_unique< Attacker >( 5 );
        unique_player->ai = std::make_unique< PlayerAI >();
        unique_player->pickable = nullptr;
        unique_player->inventory = std::make_unique< Inventory >( 26 );
        this->player = unique_player.get();

        current_area->add( CREATURES, std::move( unique_player ) );
    }

    void Engine::spawnAgreeMenu()
    {
        Menu::switchCurrentMenu( agree_menu.get() );
        Engine::setState( MENU );
    }

    void Engine::spawnMainMenu()
    {
        Menu::switchCurrentMenu( main_menu.get() );
        Engine::setState( MENU );
    }

    void Engine::spawnGameMenu()
    {
        Menu::switchCurrentMenu( game_menu.get() );
        Engine::setState( MENU );
    }

    void Engine::returnState()
    {
        game_state = IDLE;
//      GameStatus temp = temp_state;
//      temp_state = game_state;
//      game_state = temp;
    }

    void Engine::setState( GameStatus state )
    {
        // Handle special cases 
        switch (state)
        {
            case MENU:
                break;
            default:
                break;
        }

//      temp_state = game_state;
        game_state = state;
    }

    Mouse Engine::getAbsoluteMouse()
    { 
        return camera.getAbsoluteMouse( current_mouse ); 
    }

    bool Engine::changeMap( bool in_dungeon, int tile_x, int tile_y )
    {
        world.setInDungeon( in_dungeon );
        if ( in_dungeon ) 
        {
            Area *temp_area = current_area; // save reference to old area 
            current_area = world.getArea();
            temp_area->moveTo( CREATURES, player, temp_area, current_area );
            current_area->computeFov( *player, current_area->getVisibility() );
            return true;
        }
        else // if on surface
        {
            Area *temp_area = current_area; // save reference to old area 
            if ( world.switchTo( tile_x, tile_y, &(player->x), &(player->y), EDGE) )
            {
                current_area = world.getArea();
                temp_area->moveTo( CREATURES, player, temp_area, current_area );
                return true;
            }
        }
        return false;
    }

    void Engine::sendToBack( ContainerType type, Actor &actor )
    {
        current_area->moveToAt( type, &actor, current_area, current_area, 0 );
    }

    bool Engine::pickATile( int *x, int *y, float max_range )
    {
        while ( ! TCODConsole::isWindowClosed() ) 
        {
            // highlight possible range
            for ( int cx = 0; cx < current_area->getWidth(); cx++ )
            {
                for ( int cy = 0; cy < current_area->getHeight(); cy++ )
                {
                    Mouse absolute_mouse = camera.getAbsoluteMouse( current_mouse );
                    if ( current_area->isInFov( cx, cy ) && 
                        ( max_range == 0 || player->getDistance( cx, cy ) <= max_range ) )
                    {
                        TCODColor color = camera.getCharBackground( cx, cy );
                        color = color * 1.2;
                        camera.setCharBackground( cx, cy, &color );
                    }
                    if ( ( current_area->isInFov( absolute_mouse.x, absolute_mouse.y ) ) && 
                         ( max_range == 0 || 
                           player->getDistance( 
                                absolute_mouse.x, absolute_mouse.y 
                           ) <= max_range ) )
                    {
                        camera.setCharBackground( absolute_mouse.x, absolute_mouse.y, 
                            &TCODColor::white );
                        if ( current_mouse.lbutton_pressed ) 
                        {
                            *x = absolute_mouse.x;
                            *y = absolute_mouse.y;
                            return true;
                        }
                    }
                }
            }

            getEngine()->render();
            flush();

            TCODSystem::waitForEvent( TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE, 
                &current_key, &current_mouse, true );

            if ( current_mouse.rbutton_pressed || current_key.vk != TCODK_NONE )
            {
                return false;
            }
        }
        return false;
    }

    Actor* Engine::getClosestActor( int x, int y, float range, bool player_included )
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
        if ( game_state == MENU )
        {
            while ( game_state == MENU && !( TCODConsole::isWindowClosed() ) )
            {
                TCODConsole::root->clear();
                Menu::renderCurrentMenu( TCODConsole::root );
                flush();
                TCODSystem::waitForEvent( TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE, 
                    &current_key, &current_mouse, true);
                Menu::updateCurrentMenu( current_key, current_mouse );
            }
            returnState(); // return to previous state
            return;
        }
        if ( game_state == STARTUP )
        {
            current_area->computeFov( *player, current_area->getVisibility() );
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
        
        // Handle Special Keys
        if ( current_key.vk == Options::getOptions().MENU )
        {
            spawnGameMenu();
            Engine::setState( MENU );
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

        // Update Camera ( Recenter )
        camera.update();
    }

    void Engine::render()
    {
        TCODConsole::root->clear();
        camera.clear();

        // Render Map
        current_area->render( camera );

        // Render items
        for ( auto&& actor : current_area->getContainer( ITEMS ) )
        {
            if ( current_area->isInFov( actor->x, actor->y ) )
            {
                actor->render( camera );
            }
        }

        // Render creatures
        for ( auto&& actor : current_area->getContainer( CREATURES ) )
        {
            if ( current_area->isInFov( actor->x, actor->y ) )
            {
                if ( actor.get() != player )
                {
                    actor->render( camera );
                }
            }
        }

        // Render the player
        player->render( camera );

        // Render the camera
        camera.render();

        // Show log and stats
        console->render();

    }

    void Engine::flush()
    {
        TCODConsole::flush();
//      camera.flush();
    }
};

