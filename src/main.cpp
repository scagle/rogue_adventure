#include "engine.hpp"

#include <libtcod/libtcod.hpp>
#include <chrono>
#include <thread>

cursed::Engine engine(100, 50);

int main() 
{
    while ( !TCODConsole::isWindowClosed() ) 
    {
        auto begin = std::chrono::steady_clock::now();

        engine.update();
        engine.render();
        TCODConsole::flush();

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin );
        std::this_thread::sleep_for( std::chrono::milliseconds( 25 ) - duration );
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

