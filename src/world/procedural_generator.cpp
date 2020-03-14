#include "procedural_generator.hpp"

#include "zone.hpp"

#include "../datatypes/tile.hpp"

#include <libtcod/libtcod.hpp>

namespace rogue_adventure
{
    // Static Declaration

    // Constructors
    ProceduralGenerator::ProceduralGenerator()
    {
        
    }

    // Methods
    std::unique_ptr< Zone > ProceduralGenerator::createRandomZone( int seed )
    {
        std::unique_ptr< TCODRandom > rng = std::make_unique< TCODRandom >( seed, TCOD_RNG_CMWC );
        std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles = 
            std::make_unique< std::array< std::array< Tile, 100 >, 100 > >();
        
        // Perform procedures
        randomWaller( *(tiles.get()), 100, 100, rng, 20 );

        return std::make_unique< Zone >( std::move( tiles ), 100, 100 );
    }

    // Randomly creates walls with a ( 1 / ( bareness ) ) chance
    void ProceduralGenerator::randomWaller( std::array< std::array< Tile, 100 >, 100 >& tiles, 
        int width, int height, std::unique_ptr< TCODRandom >& seeded_rng, int bareness )
    {
        for ( int x = 0; x < width; x++ )
        {
            for ( int y = 0; y < height; y++ )
            {
                if ( seeded_rng->getInt( 1, bareness ) == 1 )
                {
                    tiles[x][y] = Tile('#');
                }
                else
                {
                    tiles[x][y] = Tile(' ');
                }
            }
        }
    }
};
