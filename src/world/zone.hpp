#pragma once

namespace rogue_adventure
{
    class Camera;
};

#include "area.hpp"
#include "map.hpp"
#include "../datatypes/tile.hpp"

#include <memory>

namespace rogue_adventure
{
    class Zone : public Area 
    {
        private:
        std::vector< std::unique_ptr< Map > > maps;
        int current_index = 0;

        void addMaps();
        
        public:
        Zone( std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles, int width, int height);
        virtual ~Zone() { }

        bool isInFov( int x, int y ) const;
        bool isExplored( int x, int y ) const;
        void computeFov( Actor &observer, int fov_radius );

        Map* getMap() { return this->maps[current_index].get(); }

        virtual void render( Camera &camera ) const;
    };
};
