#pragma once

#include "zone.hpp"
#include "procedural_generator.hpp"

#include "../enums/entry_point.hpp"

#include <memory>
#include <utility>
#include <array>

namespace cursed
{
    class World
    {
        // Start out with 2x2 world for simplicity
        std::array< std::unique_ptr< Zone >, 4 > zones; 
        int width = 2;
        int height = 2;
        static Zone* current_zone;
        static int current_index;

        ProceduralGenerator generator;

        public:
        World();
        virtual ~World() { }

        // Create a random world with the given seed
        void createWorld( int seed );

        // Switch to a different map, and move player to appropriate position
        bool switchTo( int tile_x, int tile_y, int *player_x, int *player_y, EntryPoint entry = CENTER );

        static Zone& getZone() { return *current_zone; }
    };
};
