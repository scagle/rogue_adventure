#include "container_component.hpp"

#include "actor.hpp"
#include "enums/container_type.hpp"

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

    // Initialize a container type pair
    bool ContainerComponent::create( ContainerType type )
    {
        if ( container_map.find( type ) == container_map.end() )
        {
            container_map.insert( 
                std::make_pair( type, std::vector< std::unique_ptr< Actor > >() ) 
            );
            return true;
        }
        return false;
    }

    // Erase first match from any ContainerComponent
    bool ContainerComponent::erase( ContainerType type, Actor *target )
    {
        for ( auto *component : containers )
        {
            if ( eraseFrom( type, target, component ) )
            {
                return true;
            }
        }
        
        printf("*** WARNING: Couldn't find target actor anywhere! (container_component.cpp)\n");
        return false;
    }

    // Erase first match from specific ContainerComponent
    bool ContainerComponent::eraseFrom( ContainerType type, Actor *target, ContainerComponent *component )
    {
        int i = 0;
        auto&& container = component->getContainer( type );
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
    bool ContainerComponent::add( ContainerType type, std::unique_ptr< Actor > target )
    {
        this->container_map[type].push_back( std::move( target ) );
        return true;
    }

    // Add to this ContainerComponent at specific index
    bool ContainerComponent::addAt( ContainerType type, std::unique_ptr< Actor > target, int index )
    {
        container_map[type].insert( container_map[type].begin() + index, std::move( target ) ); 
        return true;
    }

    // Add to specific ContainerComponent at end
    bool ContainerComponent::addTo( ContainerType type, std::unique_ptr< Actor > target, 
        ContainerComponent *component )
    {
        component->getContainer( type ).push_back( std::move( target ) );
        return true;
    }

    // Add to specific ContainerComponent at specific index
    bool ContainerComponent::addToAt( ContainerType type, std::unique_ptr< Actor > target, 
        ContainerComponent *component, int index )
    {
        auto&& container = component->getContainer( type );
        container.insert( container.begin() + index, std::move( target ) ); 
        return true;
    }

    // Move specific Actor from one specific ContainerComponent to another one
    bool ContainerComponent::moveTo( ContainerType type, Actor *target, ContainerComponent *source, 
        ContainerComponent *dest )
    {
        auto&& sc = source->getContainer( type );
        auto&& dc = dest->getContainer( type );
        int i = 0;
        for ( auto&& actor : sc )
        {
            if ( actor.get() == target )
            {
                auto temp = std::move( sc[i] );  // extra move in case of self write ( sendToBack )
                sc.erase( sc.begin() + i );
                dc.push_back( std::move( temp ) );
                return true;
            }
            i++;
        }
        return false;
    }

    // Move specific Actor from one specific ContainerComponent to another one at a specific index
    bool ContainerComponent::moveToAt( ContainerType type, Actor *target, ContainerComponent *source, 
        ContainerComponent *dest, int index )
    {
        auto&& sc = source->getContainer( type );
        auto&& dc = dest->getContainer( type );
        int i = 0;
        for ( auto&& actor : sc )
        {
            if ( actor.get() == target )
            {
                auto temp = std::move( sc[i] );  // extra move in case of self write ( sendToBack )
                sc.erase( sc.begin() + i );
                dc.insert( dc.begin() + index, std::move( temp ) );
                return true;
            }
            i++;
        }
        return false;
    }
    std::unique_ptr< std::vector< Actor* > > ContainerComponent::getAllActors()
    {
        std::unique_ptr< std::vector< Actor* > > all_actors = 
            std::make_unique< std::vector< Actor* > >();
        for ( auto const& pair : container_map )
        {
            for ( auto const& actor : pair.second )
            {
                all_actors->push_back( actor.get() );
            }
        }
        return std::move( all_actors );
    }
};
