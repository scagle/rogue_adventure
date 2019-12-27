#pragma once

namespace cursed
{
    class Actor;
};

#include <utility>
#include <memory>
#include <vector>

namespace cursed
{
    class ContainerComponent
    {
        private:
        static std::vector< ContainerComponent* > containers;

        protected:
        std::vector< std::unique_ptr< Actor > > container;
        int max_size = 10;

        public:
        ContainerComponent( int max_size ); 
        virtual ~ContainerComponent();

        std::vector< std::unique_ptr< Actor > >& getContainer() { return this->container; }

        bool erase( Actor *target );
        bool eraseFrom( Actor *target, ContainerComponent *component );
        void add( std::unique_ptr< Actor > target );
        void addTo( std::unique_ptr< Actor > target, ContainerComponent *component );
        void addToAt( std::unique_ptr< Actor > target, ContainerComponent *component, int index );
        void moveTo( Actor *target, ContainerComponent *source, ContainerComponent *dest );
        void moveToAt( Actor *target, ContainerComponent *source, ContainerComponent *dest, int index );

    };
};
