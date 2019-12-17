#pragma once

#include "map.hpp"

#include <vector>

namespace cursed
{
    class ResourceHandler
    {
        std::vector< Map > maps;

        bool loadMaps();

        public:
        ResourceHandler() { }
        virtual ~ResourceHandler() { }

        bool loadResources();

        Map* getMap( int index ) { return &( this->maps[index] ); }
    };
};
