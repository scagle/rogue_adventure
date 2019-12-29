#pragma once

#include "area.hpp"
#include "map.hpp"
#include "../datatypes/tile.hpp"

#include <memory>

namespace cursed
{
    class Zone : public Area 
    {
        private:
        std::vector< std::unique_ptr< Map > > maps;
        int current_index = 0;
        
        public:
        Zone( std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles, int width, int height);
        virtual ~Zone() { }

        Map* getMap() { return this->maps[current_index].get(); }

        void addMaps();

        virtual void render() const;
    };
};
