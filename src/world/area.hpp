#pragma once
namespace cursed
{
    class Tile;
};


#include "../container_component.hpp"
//#include "../datatypes/tile.hpp"

namespace cursed
{
    // Area is an in-game area where player roams about 
    class Area : public ContainerComponent
    {
        protected:
        std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles;
        int width;
        int height;

        public:
        Area( std::unique_ptr< std::array< std::array< Tile, 100 >, 100 > > tiles, int width, int height);
//      Area( Area const& other );
        virtual ~Area() { }

        int getWidth() const { return this->width; }
        int getHeight() const { return this->height; }

        virtual void render() const = 0;
    };
};
