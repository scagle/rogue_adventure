#include "engine.hpp"

#include <libtcod/libtcod.hpp>
#include <chrono>
#include <thread>

cursed::Engine engine(80, 80);

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
        std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) - duration );
    }
    return 0;
}

