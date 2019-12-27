#include "container_component.hpp"

#include "actor.hpp"

#include <vector> 
#include <algorithm> 

namespace cursed
{
    // Static Declaration
    std::vector< ContainerComponent* > ContainerComponent::containers;

    // Constructors
    ContainerComponent::ContainerComponent( int max_size ) : max_size( max_size ) 
    { 
        containers.push_back( this ); 
    }

    ContainerComponent::~ContainerComponent() 
    { 
        containers.erase( std::remove( containers.begin(), containers.end(), this ), 
            containers.end() );
    }

    // Erase first match from any ContainerComponent
    bool ContainerComponent::erase( Actor *target )
    {
        for ( auto *component : containers )
        {
            if ( eraseFrom( target, component ) )
            {
                return true;
            }
        }
        
        printf("*** WARNING: Couldn't find target actor anywhere! (container_component.cpp)\n");
        return false;
    }

    // Erase first match from specific ContainerComponent
    bool ContainerComponent::eraseFrom( Actor *target, ContainerComponent *component )
    {
        int i = 0;
        auto&& container = component->getContainer();
        for ( auto&& actor : container )
        {
            if ( actor.get() == target )
            {
                container.erase( container.begin() + i );
                return true;
            }
            i++;
        }

        return false;
    }

    // Add to this ContainerComponent at end
    void ContainerComponent::add( std::unique_ptr< Actor > target )
    {
        this->container.push_back( std::move( target ) );
    }

    // Add to this ContainerComponent at specific index
    void ContainerComponent::addTo( std::unique_ptr< Actor > target )
    {
        container.insert( container.begin() + index, std::move( target ) ); 
    }

    // Add to specific ContainerComponent at end
    void ContainerComponent::addTo( std::unique_ptr< Actor > target, 
        ContainerComponent *component )
    {
        component->getContainer().push_back( std::move( target ) );
    }

    // Add to specific ContainerComponent at specific index
    void ContainerComponent::addToAt( std::unique_ptr< Actor > target, 
        ContainerComponent *component, int index )
    {
        auto&& container = component->getContainer();
        container.insert( container.begin() + index, std::move( target ) ); 
    }

    // Move specific Actor from one specific ContainerComponent to another one
    void ContainerComponent::moveTo( Actor *target, ContainerComponent *source, 
        ContainerComponent *dest )
    {
        auto&& sc = source->getContainer();
        auto&& dc = dest->getContainer();
        int i = 0;
        for ( auto&& actor : sc )
        {
            if ( actor.get() == target )
            {
                dc.push_back( std::move( sc[i] ) );
                sc.erase( sc.begin() + i );
                return;
            }
            i++;
        }
    }

    // Move specific Actor from one specific ContainerComponent to another one at a specific index
    void ContainerComponent::moveToAt( Actor *target, ContainerComponent *source, 
        ContainerComponent *dest, int index )
    {
        auto&& sc = source->getContainer();
        auto&& dc = dest->getContainer();
        int i = 0;
        for ( auto&& actor : sc )
        {
            if ( actor.get() == target )
            {
                dc.insert( dc.begin() + index, std::move( sc[i] ) );
                sc.erase( sc.begin() + i );
                return;
            }
            i++;
        }
    }
};
