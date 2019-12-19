#include "map.hpp"

#include "engine.hpp"
#include "engine.hpp"

#include "datatypes/tile.hpp"

namespace cursed
{
    Map::Map( Engine *engine, int width, int height, Tile *tiles )
    {
        construct( engine, width, height, tiles );
    }

    Map::Map( Map const &other )
    {
        construct( other.engine, other.width, other.height, other.tiles );
    }

    Map::~Map() 
    { 
        delete[] tiles; 
        delete fov_map; 
        for (auto actor : this->actors)
        {
            delete actor;
        }
    }


    void Map::construct( Engine *engine, int width, int height, Tile *tiles )
    {
        this->engine = engine;
        this->width = width;
        this->height = height;
        this->tiles = new Tile[width * height];
        this->fov_map = new TCODMap( width, height );
        if ( tiles != nullptr )
        {
            for ( int x = 0; x < width; x++ )
            {
                for ( int y = 0; y < height; y++ )
                {
                    this->tiles[ x + y*width ] = *( tiles + ( x + y*width ) );
                    fov_map->setProperties( 
                        x, y, 
                        this->tiles[ x + y*width ].walkable, 
                        this->tiles[ x + y*width ].transparent 
                    );
                }
            }
        }
    }

    bool Map::isWall( int x, int y ) const
    {
        return !fov_map->isWalkable( x, y );
    }

    bool Map::isWalkable( int x, int y ) const
    {
        // Check for wall
        if ( isWall( x, y ) ) 
        {
            return false;
        }

        // Check for actor collision
        for ( auto *actor : actors )
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
        if ( fov_map->isInFov( x, y ) )
        {
            tiles[x + y*width].explored = true;
            return true;
        }
        return false;
    }

    bool Map::isExplored( int x, int y ) const
    {
        return ( tiles[x +  y*width].explored );
    }

    void Map::computeFov( Actor* observer, int fov_radius )
    {
        fov_map->computeFov( observer->x, observer->y, fov_radius );
    }


    void Map::render() const
    {
        for ( int x = 0; x < width; x++ )
        {
            for ( int y = 0; y < height; y++ )
            {
                Tile *tile = &( tiles[x + y*width] );
                TCODConsole::root->setChar( x, y, tile->code );
                if ( isInFov( x, y ) )
                {
                    TCODConsole::root->setCharForeground( x, y, tile->fg );
                    TCODConsole::root->setCharBackground( x, y, tile->bg );
                } 
                else
                {
                    // Check if tile has been explored yet
                    if ( tiles[x + y*width].explored )
                    {
                        TCODConsole::root->setCharForeground( x, y, tile->getOffsetColor( FG, 0.5 ) );
                        TCODConsole::root->setCharBackground( x, y, tile->getOffsetColor( BG, 0.5 ) );
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
