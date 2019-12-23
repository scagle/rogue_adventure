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
        if ( size > 0 && inventory.size() >= size )
        {
            return false;
        }
        inventory.push_back( actor );
        return true;
    }

    void Container::remove( Actor *actor )
    {
        inventory.erase( std::remove( inventory.begin(), inventory.end(), actor ), 
            inventory.end() );

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
