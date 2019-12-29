#pragma once

namespace cursed
{
    class Actor;
};

#include "enums/container_type.hpp"

#include <utility>
#include <memory>
#include <vector>
#include <unordered_map>

namespace cursed
{
    const int INFINITE = 0; // Infinite slots in inventory TODO

    class ContainerComponent
    {
        private:
        static std::vector< ContainerComponent* > containers;

        protected:
        std::unordered_map< ContainerType, std::vector< std::unique_ptr< Actor > > > container_map;
        int max_size = 10;

        public:
        ContainerComponent( int max_size ); 
        virtual ~ContainerComponent();

        bool create( ContainerType type );
        bool erase( ContainerType type, Actor *target );
        bool eraseFrom( ContainerType type, Actor *target, ContainerComponent *component );
        bool add( ContainerType type, std::unique_ptr< Actor > target );
        bool addAt( ContainerType type, std::unique_ptr< Actor > target, int index );
        bool addTo( ContainerType type, std::unique_ptr< Actor > target, ContainerComponent *component );
        bool addToAt( ContainerType type, std::unique_ptr< Actor > target, ContainerComponent *component, int index );
        bool moveTo( ContainerType type, Actor *target, ContainerComponent *source, ContainerComponent *dest );
        bool moveToAt( ContainerType type, Actor *target, ContainerComponent *source, ContainerComponent *dest, int index );

        std::vector< std::unique_ptr< Actor > >& getContainer( ContainerType type );
        std::unique_ptr< std::vector< Actor* > > getAllActors( );

    };
};
