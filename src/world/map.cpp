#include "map.hpp"

#include "../character/actor.hpp"
#include "../datatypes/tile.hpp"

namespace cursed
{
    Map::Map( std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles, 
        int width, int height )
        : Area( std::move( tiles ), width, height )
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
//      this->width = width;
//      this->height = height;
//      this->tiles = new Tile[width * height];
        this->fov_map = std::make_unique< TCODMap >( width, height );
        if ( tiles != nullptr )
        {
            for ( int x = 0; x < width; x++ )
            {
                for ( int y = 0; y < height; y++ )
                {
//                  this->tiles[ x + y*width ] = *( tiles + ( x + y*width ) );
                    fov_map->setProperties( 
                        x, y, 
                        (*tiles)[x][y].walkable, 
                        (*tiles)[x][y].transparent 
                    );
                }
            }
        }
    }

    bool Map::isWall( int x, int y ) const
    {
        return !fov_map->isWalkable( x, y );
    }

    bool Map::isWalkable( int x, int y )
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


    void Map::render() const
    {
        for ( int x = 0; x < width; x++ )
        {
            for ( int y = 0; y < height; y++ )
            {
                Tile &tile = (*tiles)[x][y];
                TCODConsole::root->setChar( x, y, tile.code );
                if ( isInFov( x, y ) )
                {
                    TCODConsole::root->setCharForeground( x, y, tile.fg );
                    TCODConsole::root->setCharBackground( x, y, tile.bg );
                } 
                else
                {
                    // Check if tile has been explored yet
                    if ( (*tiles)[x][y].explored )
                    {
                        TCODConsole::root->setCharForeground( x, y, 
                            tile.getOffsetColor( FG, 0.5 ) );
                        TCODConsole::root->setCharBackground( x, y, 
                            tile.getOffsetColor( BG, 0.5 ) );
                    }
                    else
                    {
                        TCODConsole::root->setCharForeground( x, y, TCODColor::black );
                        TCODConsole::root->setCharBackground( x, y, TCODColor::black );
                    }
                } 
            }
        }
    }
};
