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

    TextGUI::TextGUI( Menu *menu, int x, int y, int width, int height )
        : GUI(menu, x, y, width, height), Textable()
    {
        
    }

    ButtonGUI::ButtonGUI( Menu *menu, int x, int y, int width, int height )
        : GUI(menu, x, y, width, height), Textable(), Pressable()
    {
        
    }

    InputGUI::InputGUI( Menu *menu, int x, int y, int width, int height )
        : GUI(menu, x, y, width, height), Textable(), Pressable(), Focusable()
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

    void TextGUI::update()
    {
        
    }

    void ButtonGUI::update()
    {
        
    }

    void InputGUI::update()
    {
        
    }

    void TextGUI::render( TCODConsole *console, bool is_parent, GUI* focused_gui )
    {
        if ( is_parent )
        {
            GUI::render( console, is_parent, focused_gui );
            return;
        }

        Bound cbound = bound.getCentered();
        console->printRectEx( cbound.x, cbound.y, cbound.w, cbound.h, 
            TCOD_BKGND_NONE, TCOD_CENTER, getText().c_str() );
    }

    void ButtonGUI::render( TCODConsole *console, bool is_parent, GUI* focused_gui )
    {
        if ( is_parent )
        {
            GUI::render( console, is_parent, focused_gui );
            return;
        }

        if ( focused_gui == this ) 
        {
            TCODColor old_color = console->getDefaultBackground();
            console->setDefaultBackground( getSelectedColor() );
            console->rect( bound.x, bound.y, bound.w, bound.h, true, TCOD_BKGND_SET );
            console->setDefaultBackground( old_color );
        }

        Bound cbound = bound.getCentered();
        console->printRectEx( cbound.x, cbound.y, cbound.w, cbound.h, 
            TCOD_BKGND_NONE, TCOD_CENTER, getText().c_str() );
    }

    void InputGUI::render( TCODConsole *console, bool is_parent, GUI* focused_gui )
    {
        if ( is_parent )
        {
            GUI::render( console, is_parent, focused_gui );
            return;
        }

        if ( !isFocused() && focused_gui == this ) 
        {
            TCODColor old_color = console->getDefaultBackground();
            console->setDefaultBackground( getSelectedColor() );
            console->rect( bound.x, bound.y, bound.w, bound.h, true, TCOD_BKGND_SET );
            console->setDefaultBackground( old_color );
        }
        
        if ( isFocused() )
        {
            TCODColor old_color = console->getDefaultBackground();
            console->setDefaultBackground( getFocusColor() );
            console->rect( bound.x, bound.y, 3, 3, true, TCOD_BKGND_SET );
            console->rect( bound.x + bound.w - 3, bound.y, 3, 3, true, TCOD_BKGND_SET );
            console->printRectEx( bound.x+1, bound.y+1, 1, 1, 
                TCOD_BKGND_NONE, TCOD_CENTER, "-" );
            console->printRectEx( bound.x + bound.w - 2, bound.y+1, 1, 1, 
                TCOD_BKGND_NONE, TCOD_CENTER, "+" );
            console->setDefaultBackground( old_color );
        }

        Bound cbound = bound.getCentered();
        console->printRectEx( cbound.x, cbound.y, cbound.w, cbound.h, 
            TCOD_BKGND_NONE, TCOD_CENTER, getText().c_str() );
    }

    void GUI::render( TCODConsole *console, bool is_parent, GUI* focused_gui )
    {
        // Print the frame of parent gui
        console->printFrame( 0, 0, console->getWidth(), console->getHeight(), true, TCOD_BKGND_DEFAULT, title.c_str() );

        // Render all children
        for ( auto&& child : children )
        {
            child->render( console, false, focused_gui );
        }
    }

    GUI* GUI::getPressableChild( int child )
    {
        int valid_child_count = 0;
        int index = 0;
        while ( index < children.size() )
        {
            if ( children[index]->isPressable() )
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

    void ButtonGUI::press()
    {
        menu->action( this, getAction() );
    }
};
