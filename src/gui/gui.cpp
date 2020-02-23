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

    SliderGUI::SliderGUI( Menu *menu, int x, int y, int width, int height )
        : GUI(menu, x, y, width, height), Textable(), Pressable(), Focusable()
    {
        
    }

    TextInputGUI::TextInputGUI( Menu *menu, int x, int y, int width, int height )
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

    void GUI::update( TCOD_key_t &key, TCOD_mouse_t &mouse )
    {
        
    }

    void TextGUI::update( TCOD_key_t &key, TCOD_mouse_t &mouse )
    {
        
    }

    void ButtonGUI::update( TCOD_key_t &key, TCOD_mouse_t &mouse )
    {
        
    }

    void SliderGUI::update( TCOD_key_t &key, TCOD_mouse_t &mouse )
    {
        
    }

    void TextInputGUI::update( TCOD_key_t &key, TCOD_mouse_t &mouse )
    {
        TCOD_keycode_t special_key = key.vk;
        int regular_key = key.c;

        // Special Keys
//      if ( special_key == Options::getOptions().MENU ||
//           special_key == Options::getOptions().MENU_SELECT )
//      {
//          escaped = true;
//      }

        // Regular Keys
        if ( regular_key != 0 )
        {
            if ( regular_key == 8 )
            {
                if ( ! input_text.empty())
                {
                    input_text.pop_back();
                }
            }
            else if ( regular_key >= 33 && regular_key <= 126 ) 
            {
                input_text.append( 1, regular_key );
            }
        }
//      escaped = false;
    }

    void TextGUI::render( TCODConsole *console, bool is_parent, GUI* focused_gui )
    {
        if ( is_parent )
        {
            GUI::render( console, is_parent, focused_gui );
            return;
        }

        Textable::render( console, this );
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
            Pressable::render( console, this );
        }

        Textable::render( console, this );
    }

    void SliderGUI::render( TCODConsole *console, bool is_parent, GUI* focused_gui )
    {
        if ( is_parent )
        {
            GUI::render( console, is_parent, focused_gui );
            return;
        }

        if ( !Focusable::isFocused() && focused_gui == this ) 
        {
            Pressable::render( console, this );
        }
        
        if ( Focusable::isFocused() )
        {
            renderFocused( console );
        }

        Textable::render( console, this );
    }

    void TextInputGUI::render( TCODConsole *console, bool is_parent, GUI* focused_gui )
    {
        if ( is_parent )
        {
            GUI::render( console, is_parent, focused_gui );
            return;
        }

        if ( !Focusable::isFocused() && focused_gui == this ) 
        {
            Pressable::render( console, this );
        }
        
        if ( Focusable::isFocused() )
        {
            renderFocused( console );
        }
        else
        {
            Textable::render( console, this, input_text );
        }
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

    void SliderGUI::renderFocused( TCODConsole *console )
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

    void TextInputGUI::renderFocused( TCODConsole *console )
    {
        TCODColor old_color = console->getDefaultBackground();
        console->setDefaultBackground( TCODColor::darkGrey );
        console->rect( bound.x, bound.y, bound.w, bound.h, true, TCOD_BKGND_SET );
        console->setDefaultBackground( old_color );

        Bound cbound = bound.getCentered();
        console->printRectEx( cbound.x, cbound.y, cbound.w, cbound.h, 
            TCOD_BKGND_NONE, TCOD_CENTER, input_text.c_str() );
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

    void ButtonGUI::select()
    {
        menu->action( this, getAction() );
    }

    void SliderGUI::select()
    {
        toggleFocus(); 
    }

    void TextInputGUI::select()
    {
        toggleFocus(); 
    }
};
