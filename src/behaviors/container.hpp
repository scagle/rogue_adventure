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
        std::vector< Actor* > inventory;

        public:
        Container( int size );
        virtual ~Container() { }

        bool add( Actor* actor );
        void remove( Actor *actor );

        std::vector< Actor* >& getInventory() { return this->inventory; }
    };
};
