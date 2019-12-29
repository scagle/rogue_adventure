#include "area.hpp"

#include "../container_component.hpp"
#include "../character/actor.hpp"
#include "../datatypes/tile.hpp"

namespace cursed
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
        return !(*tiles)[x][y].walkable;
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
