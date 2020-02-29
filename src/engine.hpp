#pragma once

#include "character/actor.hpp"
#include "datatypes/mouse.hpp"
#include "enums/game_status.hpp"
#include "gui/console.hpp"
#include "gui/menu.hpp"
#include "world/world.hpp"
#include "world/map.hpp"

#include "camera.hpp"
#include "resource_handler.hpp"

#include <vector>
#include <utility>

namespace cursed
{
    class Engine
    {
        private:
        static bool close_game; // close game
        static int screen_width;
        static int screen_height;

        static Engine *active_engine;
        static World world;   // World which contains all the maps/zones
        static Camera camera; // Player's camera viewport
//      static ResourceHandler resource_handler;
        static Area *current_area;        
        static int map_visibility;
        static GameStatus game_state, temp_state; // Current game state, and temp storage state
        static TCOD_key_t current_key;
        static TCOD_mouse_t current_mouse;
        static std::unique_ptr< Console > console;
        static std::unique_ptr< AgreeMenu > agree_menu;
        static std::unique_ptr< MainMenu > main_menu;
        static std::unique_ptr< GameMenu > game_menu;
        static Actor *player; // pointer to the unique_player in map

        public:
        Engine( int screen_width, int screen_height );
        virtual ~Engine();

        void init();
        void initMenus();
        void createMainCharacter( std::string name );
        void load();
        void save();

        static int getScreenWidth() { return screen_width; }
        static int getScreenHeight() { return screen_height; }
        static Engine* getEngine() { return active_engine; }
        static Area& getArea() { return *current_area; }
        static Actor& getPlayer() { return *player; }
        static World& getWorld() { return world; }
        static std::vector< std::unique_ptr< Actor > >& getActors() { return current_area->getContainer( CREATURES ); }
        static std::vector< std::unique_ptr< Actor > >& getItems() { return current_area->getContainer( ITEMS ); }
        static int getVisibility() { return map_visibility; }
        static TCOD_key_t& getCurrentKey() { return current_key; }
        static TCOD_mouse_t& getCurrentMouse() { return current_mouse; }
        static GameStatus getState() { return game_state; }
        static Console& getConsole() { return *console; }
        static AgreeMenu* getAgreeMenu() { return agree_menu.get(); }
        static MainMenu* getMainMenu() { return main_menu.get(); }
        static GameMenu* getGameMenu() { return game_menu.get(); }
        static bool isGameClosed() { return close_game; } 

        static void spawnAgreeMenu();
        static void spawnMainMenu(); 
        static void spawnGameMenu(); 

        static void returnState();
        static void setState( GameStatus state ); 
        static void setInDungeon( bool in_dungeon ) { world.setInDungeon( in_dungeon ); }
        static void closeGame() { close_game = true; } 

        static Mouse getAbsoluteMouse(); 
        static bool changeMap( bool state = false, int tile_x = 0, int tile_y = 0 );
        static void sendToBack( ContainerType type, Actor &actor );
        static void eraseActor( Actor *target );
        static bool pickATile( int *x, int *y, float max_range = 0 );
        static Actor* getClosestActor( int x, int y, float range = 0, bool player_included = true ); 

        void update( bool blocking = true );
        void render();
        static void flush();
    };
};
