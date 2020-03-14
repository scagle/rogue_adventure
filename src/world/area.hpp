#pragma once
namespace rogue_adventure
{
    class Tile;
    class Camera;
};


#include "../container_component.hpp"
//#include "../datatypes/tile.hpp"

namespace rogue_adventure
{
    // Area is an in-game area where player roams about 
    class Area : public ContainerComponent
    {
        protected:
        std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles;
        int width;
        int height;
        int visibility;

        public:
        Area( std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles, 
            int width, int height, int visibility );
//      Area( Area const& other );
        virtual ~Area() { }

        virtual bool isWall( int x, int y ) const;
        virtual bool isWalkable( int x, int y );
        virtual bool isInFov( int x, int y ) const = 0;
        virtual bool isExplored( int x, int y ) const = 0;
        virtual void computeFov( Actor &observer, int fov_radius ) = 0;

        int getWidth() const { return this->width; }
        int getHeight() const { return this->height; }
        int getVisibility() const { return this->visibility; }

        virtual void render( Camera &camera ) const = 0;
    };
};
