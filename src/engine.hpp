#pragma once

#include "actor.hpp"
#include "map.hpp"
#include "resource_handler.hpp"

#include "enums/game_status.hpp"
#include "gui/console.hpp"

#include <vector>
#include <utility>

namespace cursed
{
    class Engine
    {
        static int screen_width;
        static int screen_height;

        static Engine *active_engine;
        static ResourceHandler resource_handler;
        //static std::vector< Actor* > current_actors;     // all npcs
        //static std::vector< Actor* > current_items;      // all items
        static Map *current_map;        
        static int map_visibility;
        static GameStatus game_state;
        static TCOD_key_t current_key;
        static TCOD_mouse_t current_mouse;
        static std::unique_ptr< Console > console;
        static Actor *player;   // pointer to the unique_player in map

        public:
        Engine( int screen_width, int screen_height );
        virtual ~Engine();

        static int getScreenWidth() { return screen_width; }
        static int getScreenHeight() { return screen_height; }
        static Engine* getEngine() { return active_engine; }
        static Map& getMap() { return *current_map; }
        static Actor& getPlayer() { return *player; }
        static std::vector< std::unique_ptr< Actor > >& getActors() { return current_map->getContainer( CREATURES ); }
        static std::vector< std::unique_ptr< Actor > >& getItems() { return current_map->getContainer( ITEMS ); }
        static int getVisibility() { return map_visibility; }
        static TCOD_key_t& getCurrentKey() { return current_key; }
        static TCOD_mouse_t& getCurrentMouse() { return current_mouse; }
        static GameStatus getState() { return game_state; }
        static Console& getConsole() { return *console; }

        static void setState( GameStatus state ) { game_state = state; }
        // void setActors( Map *map );
        // TODO: void addActor( Actor* actor );

        static void sendToBack( ContainerType type, Actor &actor );
        static void eraseActor( Actor *target );
        static bool pickATile( int *x, int *y, float max_range = 0 );

        // Can return nullptr
        static Actor* getClosestActor( int x, int y, float range = 0, bool player_included = true ); 

        void update( bool blocking = true );
        void render();

        private:
    };
};
