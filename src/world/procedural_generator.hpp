#pragma once
namespace rogue_adventure
{
    class Zone;
};

#include "../datatypes/tile.hpp"

#include <memory>
#include <utility>

namespace rogue_adventure
{
    class ProceduralGenerator
    {
        
        public:
        ProceduralGenerator();
        virtual ~ProceduralGenerator() { }

        std::unique_ptr< Zone > createRandomZone( int seed );

        // Procedures
        void randomWaller( std::array< std::array< Tile, 100 >, 100 >& tiles, int width, int height, 
            std::unique_ptr< TCODRandom > &seeded_rng, int bareness );
    };
};
