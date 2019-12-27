#pragma once

namespace cursed
{
    class Actor;
}

#include <vector>
#include <memory>

namespace cursed
{
    class Container
    {
        int size; // maximum number of actors. 0 = unlimited
        std::vector< Actor* > container;

        public:
        Container( int size );
        virtual ~Container() { }

        bool add( Actor* actor );
        void remove( Actor *actor );

        std::vector< Actor* >& getContainer() { return this->container; }
    };
};

//    class Container
//    {
//        int size; // maximum number of actors. 0 = unlimited
//        std::vector< Actor* > container;
//
//        public:
//        Container( int size );
//        virtual ~Container() { }
//
//        bool add( std::unique_ptr< Actor > actor );
//        void remove( std::unique_ptr< Actor >& actor );
//        void remove( Actor *actor );
//        int contains( std::unique_ptr< Actor >& actor );
//        int contains( Actor *actor );
//
//        std::vector< Actor* >& getContainer() { return this->container; }
//    };
