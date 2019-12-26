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
        std::unique_ptr< TCODMap > fov_map;
        std::vector< std::unique_ptr< Actor > > actors;
        std::vector< std::unique_ptr< Actor > > items;

        public:
        Map( Engine* engine, int width, int height, Tile *tiles );
        Map( Map const& other );
        void construct( Engine *engine, int width, int height, Tile *tiles );
        virtual ~Map(); 

        void addActor( std::unique_ptr< Actor > actor )  
            { this->actors.push_back( std::move( actor ) ); }
        std::vector< std::unique_ptr< Actor > >& getActors() { return this->actors; }

        void addItem( std::unique_ptr< Actor > item )
            { this->items.push_back( std::move( item ) ); }
        std::vector< std::unique_ptr< Actor > >& getItems() { return this->items; }

        bool isWall( int x, int y ) const;
        bool isWalkable( int x, int y ) const;
        bool isInFov( int x, int y ) const;
        bool isExplored( int x, int y ) const;
        int getWidth() const { return this->width; }
        int getHeight() const { return this->height; }
        void computeFov( Actor &observer, int fov_radius );


        virtual void render() const;
    };
};
