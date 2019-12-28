#include "console.hpp"

#include "../engine.hpp"
#include "../datatypes/message.hpp"

#include <utility>
#include <cstdarg>
#include <fmt/format.h>

namespace cursed
{
    // Static Declaration

    // Constructors
    Console::Console( int panel_width, int panel_height )
        : width( panel_width ), height( panel_height )
    {
        console = std::make_unique< TCODConsole >( panel_width, panel_height );
    }

    // Methods
    void Console::render()
    {
        // Clear the console
        console->setDefaultBackground( TCODColor::black );
        console->clear();

        // Draw Health Bar
        renderBar( 1, 0, width-2, "HP", Engine::getPlayer().destructible->hp, 
            Engine::getPlayer().destructible->max_hp, TCODColor::lightRed, TCODColor::darkerRed ); 

        // Mouse look
        renderMouseLook();

        // Draw the messages
        int y_offset = 1;
        for ( int i = log.size() - 1; i >= 0; i-- )
        {
            console->setDefaultForeground( log[i].color );
            console->printf( 0, y_offset, log[i].text.c_str(), TCOD_BKGND_NONE, TCOD_CENTER );

            // Check to see if its old
            if ( y_offset == height)
                break;
            y_offset++;
        }


        // blit the GUI console to the root console
        TCODConsole::blit( console.get(), 0, 0, width, height, TCODConsole::root, 0, 
            Engine::getScreenHeight() - height );
    }

    // Message to console + wrappers
    void Console::message( const TCODColor &color, const char *text, ... )
    {
        va_list args;
        va_start( args, text );
        message( color, text, args );
        va_end( args );
    }

    void Console::message( const TCODColor &color, std::string text, ... )
    {
        va_list args;
        va_start( args, text );
        message( color, text.c_str(), args );
        va_end( args );
    }

    void Console::message( const TCODColor &color, const char *text, va_list args )
    {
        char buf[256];
        vsprintf( buf, text, args );

        char *line_begin = buf;
        char *line_end;
        do 
        {
            // Make room
            if ( log.size() == MAX_MSG_SIZE )
            {
                log.erase(log.begin());
            }

            // Detect end of line
            line_end = strchr( line_begin, '\n' );
            if ( line_end )
            {
                *line_end = '\0';
            }

            if ( *line_begin != '\0' )
            {
                log.push_back( Message{ color, line_begin } );
            }

            // Go to next line
            line_begin = line_end + 1;
        } while ( line_end );
    }

    void Console::renderBar( int x, int y, int width, std::string name, 
        float value, float max_value, const TCODColor &bar_color,
        const TCODColor &back_color ) 
    {
    
        // fill the background
        console->setDefaultBackground( back_color );
        console->rect( x, y, BAR_WIDTH, 1, false, TCOD_BKGND_SET );

        int hp_bar = ( int )( value / max_value * BAR_WIDTH );
        if ( value > 0  )
        {
            // draw the bar
            console->setDefaultBackground( bar_color );
            console->rect( x, y, hp_bar, 1, false, TCOD_BKGND_SET );

            // print text on top of the bar
            console->setDefaultForeground( TCODColor::white );
            console->printf( x + BAR_WIDTH / 2, y, TCOD_BKGND_NONE, TCOD_CENTER, "%s : %g/%g", 
                name.c_str(), value, max_value );

        }
    }

    void Console::renderMouseLook()
    {
        auto &mouse = Engine::getCurrentMouse();
        if ( ! Engine::getMap().isInFov( mouse.cx, mouse.cy ) )
        {
            // If mouse is out of fov nothing to render
            return;
        }

        char buf[256] = "";
        bool first = true;
        auto all_actors = std::move( Engine::getMap().getAllActors() );
        for ( Actor *actor : *all_actors )
        {
            if ( actor->x == mouse.cx && actor->y == mouse.cy )
            {
                if ( ! first ) 
                {
                    strcat( buf, ", " );
                }
                else
                {
                    first=false;
                }
                strcat( buf, actor->name.c_str() );
            }
        }

        // display the list of actors under mouse cursor
        console->setDefaultForeground(TCODColor::lightGrey);
        console->printf( width/2, 0, buf, TCOD_CENTER );
    }
};
