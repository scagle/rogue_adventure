#pragma once

#include "map.hpp"

#include <vector>

namespace cursed
{
    class ResourceHandler
    {
        std::vector< Map > maps;
        //std::vector< Actor > actors;

        bool loadMaps();

        public:
        ResourceHandler() { }

        bool loadResources();

        Map& getMap( int index ) { return this->maps[index]; }
        //std::vector< Actor >& getActors( int index ) const { return this->maps[i]; }
    };
};
