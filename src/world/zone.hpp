#pragma once

#include "../datatypes/tile.hpp"

#include <memory>

namespace cursed
{
    class Zone
    {
        public:
        std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > surface_tiles;
        int width;
        int height;
        
        public:
        Zone( std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles, int width, int height);
        virtual ~Zone() { }
    };
};
