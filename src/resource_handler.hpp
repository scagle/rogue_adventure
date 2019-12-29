#pragma once

#include "world/map.hpp"

#include <vector>

namespace cursed
{
    class ResourceHandler
    {
        std::vector< std::unique_ptr< Map > > maps;
        bool initialized = false;

        bool loadMaps();

        public:
        ResourceHandler() { }
        virtual ~ResourceHandler() { }

        bool loadResources();

        Map* getMap( int index ); 
    };
};
