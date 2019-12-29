#include "inventory.hpp"

#include "../actor.hpp"
#include "../../enums/container_type.hpp"

namespace cursed
{
    // Static Declaration

    // Constructors
    Inventory::Inventory( int size ) 
        : ContainerComponent( size )
    {
        create( ITEMS );
    }
};

//#include "container.hpp"
//#include "../actor.hpp"
//
//namespace cursed
//{
//    // Static Declaration
//
//    // Constructors
//    Inventory::Inventory( int size ) : size( size )
//    {
//        
//    }
//
//    // Methods
//    bool Inventory::add( std::unique_ptr< Actor > actor )
//    {
//        
//        if ( size > 0 && container.size() >= size )
//        {
//            return false;
//        }
//        container.push_back( std::move( actor ) );
//        return true;
//    }
//
//    void Inventory::remove( std::unique_ptr< Actor >& actor )
//    {
//        remove( actor.get() );
//    }
//
//    void Inventory::remove( Actor *actor )
//    {
//        int i = 0;
//        for ( auto&& container_actor : container )
//        {
//            if ( container_actor.get() == actor )
//            {
//                container.erase( container.begin() + i );
//                break;
//            }
//            i++;
//        }
//    }
//
//    int Inventory::contains( std::unique_ptr< Actor >& actor )
//    {
//        return contains( actor.get() );
//    }
//
//    int Inventory::contains( Actor *actor )
//    {
//        int i = 0;
//        for ( auto&& container_actor : container )
//        {
//            if ( container_actor.get() == actor )
//            {
//                return i;
//            }
//            i++;
//        }
//        return -1;
//    }
//};
