#pragma once

namespace cursed
{
    class Actor;
}

#include "../container_component.hpp"

#include <vector>
#include <memory>

namespace cursed
{
    class Inventory : public ContainerComponent
    {
        public:
        Inventory( int size );
        virtual ~Inventory() { }
    };
};

//    class Inventory
//    {
//        int size; // maximum number of actors. 0 = unlimited
//        std::vector< Actor* > container;
//
//        public:
//        Inventory( int size );
//        virtual ~Inventory() { }
//
//        bool add( std::unique_ptr< Actor > actor );
//        void remove( std::unique_ptr< Actor >& actor );
//        void remove( Actor *actor );
//        int contains( std::unique_ptr< Actor >& actor );
//        int contains( Actor *actor );
//
//        std::vector< Actor* >& getContainer() { return this->container; }
//    };
