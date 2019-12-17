#pragma once

#include "datatypes/tile.hpp"
//#include "datatypes/array2d.hpp"

namespace cursed
{
    class Map
    {
        int width;
        int height;
        Tile *tiles = nullptr; 

        public:
        Map( int width, int height, Tile *tiles );
        Map( Map const& other );
        void construct( int width, int height, Tile *tiles );
        virtual ~Map() { delete[] tiles; }

        bool isPassable( int x, int y ) const;
        virtual void render() const;
    };
};
