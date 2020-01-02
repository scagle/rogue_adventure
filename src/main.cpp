#include "engine.hpp"

#include <libtcod/libtcod.hpp>
#include <chrono>
#include <thread>

cursed::Engine engine( 80, 80 );

int main() 
{
    // Update game once on launch
    engine.load();

    engine.update( false ); // non-blocking input
    engine.render();
    engine.flush();

    while ( !TCODConsole::isWindowClosed() ) 
    {
        auto begin = std::chrono::steady_clock::now();

        engine.update( true ); // blocking input
        engine.render();
        engine.flush();

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin );
        std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) - duration );
    }

    engine.save();

    return 0;
}

