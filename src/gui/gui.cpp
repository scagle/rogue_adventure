#include "gui.hpp"
#include "menu.hpp"

namespace cursed
{
    // Static Declaration

    // Constructors
    GUI::GUI( Menu *menu, int x, int y, int width, int height )
        : menu(menu), bound({ x, y, width, height })
    {
        
    }

    // Methods
//  bool GUI::grabNextChild( std::pair< GUI*, int >* focus, int new_index )
//  {
//      if ( new_index >= 0 && new_index < children.size() )
//      {
//          *focus = { getChild(new_index), new_index };
//          return true;
//      }
//      return false;
//  }

    void GUI::update()
    {
        
    }

    void GUI::render( TCODConsole *console, bool is_parent, GUI* focused_gui )
    {
        // See if we're focused
        if ( is_parent == false )
        {
            if ( focused_gui == this ) 
            {
                TCODColor old_color = console->getDefaultBackground();
                console->setDefaultBackground( TCODColor::purple );
                console->rect( bound.x, bound.y, bound.w, bound.h, true, TCOD_BKGND_SET );
                console->setDefaultBackground( old_color );
            }

            Bound cbound = bound.getCentered();
            console->printRectEx( cbound.x, cbound.y, cbound.w, cbound.h, 
                TCOD_BKGND_NONE, TCOD_CENTER, text.c_str() );
        }
        else
        {
            // Print the frame of parent gui
            console->printFrame( 0, 0, console->getWidth(), console->getHeight(), true, TCOD_BKGND_DEFAULT, text.c_str() );

            // Render all children
            for ( auto&& child : children )
            {
                child->render( console, false, focused_gui );
            }
        }
    }

    GUI* GUI::getPressableChild( int child )
    {
        int valid_child_count = 0;
        int index = 0;
        while ( index < children.size() )
        {
            if ( children[index]->getPressComponent() )
            {
                // Since we start at zero, evaluate before incrementing
                if ( valid_child_count == child )
                {
                    return children[index].get();
                }
                valid_child_count++;
            }
            index++;
        }

        // Desired child couldn't be reached before the end of vector
        return nullptr;
    }

    void GUI::press()
    {
        if ( press_component )
            menu->action( this, press_component->action );
    }
};
