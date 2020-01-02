#include "map.hpp"

#include "../character/actor.hpp"
#include "../datatypes/tile.hpp"
#include "../camera.hpp"

namespace cursed
{
    Map::Map( std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles, 
        int width, int height )
        : Area( std::move( tiles ), width, height, 12 )
    { 
        constructFOVMap();
    }

//  Map::Map( Map const &other )
//      : Area( other )
//  {
//      constructFOVMap();
//  }

    void Map::constructFOVMap( )
    {
        this->fov_map = std::make_unique< TCODMap >( width, height );
        if ( tiles != nullptr )
        {
            for ( int x = 0; x < width; x++ )
            {
                for ( int y = 0; y < height; y++ )
                {
                    fov_map->setProperties( 
                        x, y, 
                        (*tiles)[x][y].walkable, 
                        (*tiles)[x][y].transparent 
                    );
                }
            }
        }
    }

    bool Map::isInFov( int x, int y ) const
    {
        // Check for out of boundaries ( possible with mouse look )
        if ( x < 0 || x >= width || y < 0 || y >= height )
        {
            return false;
        }
        if ( fov_map->isInFov( x, y ) )
        {
            (*tiles)[x][y].explored = true;
            return true;
        }
        return false;
    }

    bool Map::isExplored( int x, int y ) const
    {
        return ( (*tiles)[x][y].explored );
    }

    void Map::computeFov( Actor &observer, int fov_radius )
    {
        fov_map->computeFov( observer.x, observer.y, fov_radius );
    }


    void Map::render( Camera &camera ) const
    {
        for ( int x = 0; x < width; x++ )
        {
            for ( int y = 0; y < height; y++ )
            {
                Tile &tile = (*tiles)[x][y];
                if ( isInFov( x, y ) )
                {
                    camera.setChar( x, y, tile.code, &tile.fg, &tile.bg );
                } 
                else
                {
                    // Check if tile has been explored yet
                    if ( (*tiles)[x][y].explored )
                    {
                        TCODColor fg_foggy = tile.getOffsetColor( FG, 0.5 );
                        TCODColor bg_foggy = tile.getOffsetColor( BG, 0.5 );
                        camera.setChar( x, y, tile.code, &fg_foggy, &bg_foggy );
                    }
                    else
                    {
                        camera.setChar( x, y, tile.code, &TCODColor::black, &TCODColor::black );
                    }
                } 
            }
        }
    }
};
