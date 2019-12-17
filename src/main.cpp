#include "actor.hpp"
#include "resource_handler.hpp"

#include <libtcod/libtcod.hpp>

cursed::ResourceHandler resource_handler;

int main() 
{
    TCODConsole::initRoot(80, 50, "Cursed Adventure", false);

    resource_handler.loadResources();
    cursed::Map& map = resource_handler.getMap(0);

    cursed::Actor player(40, 25, '@', TCODColor::white);
    while ( !TCODConsole::isWindowClosed() ) 
    {
        TCOD_key_t key;
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

        // Update 
        switch (key.vk)
        {
            case TCODK_UP: 
                if ( map.isPassable( player.x + 0, player.y - 1 ) )
                    player.move(  0, -1 ); 
                break;
            case TCODK_RIGHT: 
                if ( map.isPassable( player.x + 1, player.y + 0 ) )
                    player.move(  1,  0 );  
                break;
            case TCODK_LEFT: 
                if ( map.isPassable( player.x - 1, player.y + 0 ) )
                    player.move( -1,  0 ); 
                break;
            case TCODK_DOWN: 
                if ( map.isPassable( player.x + 0, player.y + 1 ) )
                    player.move(  0,  1 );  
                break;
            default: break;
        }
        TCODConsole::root->clear();

        // Rendering
        map.render();
        player.render();

        TCODConsole::flush();
    }
    return 0;
}

// #include <stdio.h>
// #include <chrono>
// #include <thread>
// 
// 
// bool initialize();
// bool close();
// 
// int main()
// {
// 
//     TCOD_console_init_root(80, 50, "Cursed", false, TCOD_RENDERER_OPENGL2);
//     while (!TCODConsole::isWindowClosed()) {
//         auto begin = std::chrono::steady_clock::now();
// 
//         auto end = std::chrono::steady_clock::now();
//         auto duration = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin );
//         std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) - duration );
// 
//     }
//     TCOD_quit();
//     return 0;
// }
// 
// bool initialize() 
// {
//     return true;
// }
// 
// bool close() 
// {
//     return true;
// }
//#include "libtcod.hpp"

