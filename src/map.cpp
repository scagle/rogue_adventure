#include "map.hpp"
#include "datatypes/tile.hpp"

namespace cursed
{
    Map::Map( int width, int height, Tile *tiles )
    {
        construct( width, height, tiles );
    }

    Map::Map( Map const &other )
    {
        construct( other.width, other.height, other.tiles );
    }
    Map::~Map() 
    { 
        delete[] tiles; 
        for (auto actor : this->actors)
        {
            delete actor;
        }
    }


    void Map::construct( int width, int height, Tile *tiles )
    {
        this->width = width;
        this->height = height;
        this->tiles = new Tile[width * height];
        if ( tiles != nullptr )
        {
            for ( int x = 0; x < width; x++ )
            {
                for ( int y = 0; y < height; y++ )
                {
                    this->tiles[ x + y*width ] = *( tiles + ( x + y*width ) );
                }
            }
        }
    }

    bool Map::isPassable( int x, int y ) const
    {
        return tiles[x + y*width].passable;
    }

    void Map::render() const
    {
        for ( int x = 0; x < width; x++ )
        {
            for ( int y = 0; y < height; y++ )
            {
                TCODConsole::root->setChar( x, y, tiles[ x + y*width ].code );
                TCODConsole::root->setCharForeground( x, y, tiles[ x + y*width ].fg );
                TCODConsole::root->setCharBackground( x, y, tiles[ x + y*width ].bg );
            }
        }
    }
};
