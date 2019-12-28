#pragma once

namespace cursed
{
    class Engine;
    class Actor;
    class Tile;
}

#include "container_component.hpp"

#include <libtcod/libtcod.hpp>
#include <vector>

namespace cursed
{
    class Map : public ContainerComponent
    {
        protected:
        Engine *engine;
        int width;
        int height;
        Tile *tiles = nullptr; 
        std::unique_ptr< TCODMap > fov_map;

        public:
        Map( Engine* engine, int width, int height, Tile *tiles );
        Map( Map const& other );
        void construct( Engine *engine, int width, int height, Tile *tiles );
        virtual ~Map(); 

        bool isWall( int x, int y ) const;
        bool isWalkable( int x, int y );
        bool isInFov( int x, int y ) const;
        bool isExplored( int x, int y ) const;
        int getWidth() const { return this->width; }
        int getHeight() const { return this->height; }
        void computeFov( Actor &observer, int fov_radius );

        virtual void render() const;
    };
};
