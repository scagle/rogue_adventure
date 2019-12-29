#include "zone.hpp"

#include "../datatypes/tile.hpp"

namespace cursed
{
    // Static Declaration

    // Constructors
    Zone::Zone( std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles, 
        int width, int height )
        : surface_tiles( std::move( tiles ) ), width( width ), height( height ) 
    { 

    }

    // Methods
};
