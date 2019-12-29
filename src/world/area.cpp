#include "area.hpp"

#include "../container_component.hpp"
#include "../datatypes/tile.hpp"

namespace cursed
{
    // Static Declaration

    // Constructors
    Area::Area( std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles, 
        int width, int height )
        : ContainerComponent( INFINITE ), tiles( std::move( tiles ) ), 
          width( width ), height( height ) 
    { 

    }
//  Area::Area( Area const& other )
//      : ContainerComponent( INFINITE )
//  {
//      tiles = std::move( other.tiles );
//      width = other.width;
//      height = other.height;
//  }

    // Methods
};
