#pragma once
namespace cursed
{
    class Zone;
};

#include "../datatypes/tile.hpp"

#include <memory>
#include <utility>

namespace cursed
{
    class ProceduralGenerator
    {
        
        public:
        ProceduralGenerator();
        virtual ~ProceduralGenerator() { }

        std::unique_ptr< Zone > createRandomZone( int seed );

        // Procedures
        void randomWaller( std::array< std::array< Tile, 100 >, 100 >& tiles, int width, int height, 
            TCODRandom *seeded_rng, int bareness );
    };
};
