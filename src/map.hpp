#pragma once

namespace cursed
{
    class Engine;
    class Actor;
    class Tile;
}

#include <libtcod/libtcod.hpp>
#include <vector>

namespace cursed
{
    class Map
    {
        protected:
        Engine *engine;
        int width;
        int height;
        Tile *tiles = nullptr; 
        TCODMap *fov_map;
        std::vector< Actor* > actors;

        public:
        Map( Engine* engine, int width, int height, Tile *tiles );
        Map( Map const& other );
        void construct( Engine *engine, int width, int height, Tile *tiles );
        virtual ~Map(); 

        void addActor(Actor *actor) { this->actors.push_back(actor); }
        std::vector< Actor* >* getActors() { return &( this->actors ); }

        bool isWall( int x, int y ) const;
        bool isWalkable( int x, int y ) const;
        bool isInFov( int x, int y ) const;
        bool isExplored( int x, int y ) const;
        void computeFov( Actor *observer, int fov_radius );

        virtual void render() const;
    };
};
