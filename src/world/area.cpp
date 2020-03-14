#include "area.hpp"

#include "../container_component.hpp"
#include "../character/actor.hpp"
#include "../datatypes/tile.hpp"

namespace rogue_adventure
{
    // Static Declaration

    // Constructors
    Area::Area( std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles, 
        int width, int height, int visibility )
        : ContainerComponent( INFINITE ), tiles( std::move( tiles ) ), 
          width( width ), height( height ), visibility( visibility ) 
    { 

    }

    bool Area::isWall( int x, int y ) const
    {
        if ( x >= 0 && x < width  && y >= 0 && y < height )
        {
            return !(*tiles)[x][y].walkable;
        }
        else
        {
            return true;
        }
    }

    bool Area::isWalkable( int x, int y )
    {
        // Check for wall
        if ( isWall( x, y ) ) 
        {
            return false;
        }

        // Check for actor collision
        for ( auto&& actor : getContainer( CREATURES ) )
        {
            if ( ( actor->x == x && actor->y == y ) &&
                 ( actor->blocks ) )
            {
                return false;
            }
        }

        return true;
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
