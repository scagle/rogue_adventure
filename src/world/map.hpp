#pragma once

namespace cursed
{
    class Actor;
    class Tile;
}

#include "../world/area.hpp"

#include <libtcod/libtcod.hpp>
#include <vector>

namespace cursed
{
    // Map is a turn-based, field of vision environment where battles take place
    class Map : public Area
    {
        protected:
        std::unique_ptr< TCODMap > fov_map;

        public:
        Map( std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles, int width, int height);
//      Map( Map const& other );
        void constructFOVMap();
        virtual ~Map() { } 

        bool isWall( int x, int y ) const;
        bool isWalkable( int x, int y );
        bool isInFov( int x, int y ) const;
        bool isExplored( int x, int y ) const;
        void computeFov( Actor &observer, int fov_radius );

        virtual void render() const;
    };
};
