#pragma once

#include "datatypes/tile.hpp"
#include "actor.hpp"

namespace cursed
{
    class Map
    {
        int width;
        int height;
        Tile *tiles = nullptr; 
        std::vector< Actor* > actors;

        public:
        Map( int width, int height, Tile *tiles );
        Map( Map const& other );
        void construct( int width, int height, Tile *tiles );
        virtual ~Map(); 

        void addActor(Actor *actor) { this->actors.push_back(actor); }
        std::vector< Actor* >* getActors() { return &( this->actors ); }

        bool isPassable( int x, int y ) const;

        virtual void render() const;
    };
};
