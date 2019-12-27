#include "container.hpp"
#include "../actor.hpp"

namespace cursed
{
    // Static Declaration

    // Constructors
    Container::Container( int size ) : size( size )
    {
        
    }

    // Methods
    bool Container::add( Actor *actor )
    {
        if ( size > 0 && container.size() >= size )
        {
            return false;
        }
        container.push_back( actor );
        return true;
    }

    void Container::remove( Actor *actor )
    {
        container.erase( std::remove( container.begin(), container.end(), actor ), 
            container.end() );

        //int i = 0;
        //for ( auto&& inv_actor : inventory )
        //{
        //    if ( inv_actor.get() == actor )
        //    {
        //        inventory.erase( inventory.begin() + i );
        //        break;
        //    }
        //    i++;
        //}
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
//    Container::Container( int size ) : size( size )
//    {
//        
//    }
//
//    // Methods
//    bool Container::add( std::unique_ptr< Actor > actor )
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
//    void Container::remove( std::unique_ptr< Actor >& actor )
//    {
//        remove( actor.get() );
//    }
//
//    void Container::remove( Actor *actor )
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
//    int Container::contains( std::unique_ptr< Actor >& actor )
//    {
//        return contains( actor.get() );
//    }
//
//    int Container::contains( Actor *actor )
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
