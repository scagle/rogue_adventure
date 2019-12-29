#include "procedural_generator.hpp"

#include "zone.hpp"

#include "../datatypes/tile.hpp"

#include <libtcod/libtcod.hpp>
#include <iostream>

namespace cursed
{
    // Static Declaration

    // Constructors
    ProceduralGenerator::ProceduralGenerator()
    {
        
    }

    // Methods
    std::unique_ptr< Zone > ProceduralGenerator::createRandomZone( int seed )
    {
        TCODRandom *rng = new TCODRandom( seed, TCOD_RNG_CMWC );
        std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles = 
            std::make_unique< std::array< std::array< Tile, 100 >, 100 > >();
        
        // Perform procedures
        randomWaller( *(tiles.get()), 100, 100, rng, 5 );

        return std::make_unique< Zone >( std::move( tiles ), 100, 100 );
    }

    // Randomly creates walls with a ( 1 / ( bareness ) ) chance
    void ProceduralGenerator::randomWaller( std::array< std::array< Tile, 100 >, 100 >& tiles, 
        int width, int height, TCODRandom *seeded_rng, int bareness )
    {
        for ( int x = 0; x < width; x++ )
        {
            for ( int y = 0; y < height; y++ )
            {
                if ( !seeded_rng->getInt( 1, bareness ) == 1 )
                {
                    std::cout << '#';
                    tiles[x][y] = Tile('#');
                }
                else
                {
                    std::cout << ' ';
                    tiles[x][y] = Tile(' ');
                }
            }
            std::cout << '\n';
        }
    }
};
