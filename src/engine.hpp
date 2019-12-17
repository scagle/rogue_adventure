#pragma once

#include "actor.hpp"
#include "map.hpp"
#include "resource_handler.hpp"

#include <vector>

namespace cursed
{
    class Engine
    {
        ResourceHandler resource_handler;
        Actor player;
        Map* current_map;        
        std::vector< Actor* >* current_actors;        

        public:
        Engine();
        virtual ~Engine() { }

        void update();
        void render();
    };
};
