#pragma once

#include "actor.hpp"
#include "map.hpp"
#include "resource_handler.hpp"

#include "enums/game_status.hpp"

#include <vector>

namespace cursed
{
    class Engine
    {
        static Engine *active_engine;
        static ResourceHandler resource_handler;
        static std::vector< Actor* > current_actors;        
        static Map* current_map;        
        static Actor player;
        static int map_visibility;
        static GameStatus game_state;
        static TCOD_key_t current_key;
        int screen_width;
        int screen_height;

        public:
        Engine( int screen_width, int screen_height );
        virtual ~Engine() { }

        static Engine* getEngine() { return active_engine; }
        static Map* getMap() { return current_map; }
        static Actor* getPlayer() { return &(player); }
        static std::vector< Actor* >* getActors() { return &(current_actors); }
        static int getVisibility() { return map_visibility; }
        static TCOD_key_t* getCurrentKey() { return &(current_key); }
        static GameStatus getState() { return game_state; }
        static void setState( GameStatus state ) { game_state = state; }

        static void sendToBack( Actor *actor );

        void update();
        void render();

        private:
        void getAllActors( Map *map ); // All actors including player
    };
};
