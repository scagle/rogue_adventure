#pragma once

#include "character/actor.hpp"
#include "engine.hpp"   // Mouse

#include <array>
#include <libtcod/libtcod.hpp>

namespace cursed
{
    class Camera
    {
        protected:
        int x, y;
        int width, height;
        TCODColor fg = TCODColor::white;
        TCODColor bg = TCODColor::black;
        std::unique_ptr< TCODConsole > viewport;
//      bool updated = false; // Flag used to signal screen refresh
        
        private:
        Actor *target; // Target to follow in center of screen ( overrides x/y coordinates )

        public:
//      Camera( int x, int y, int width, int height );
        Camera();
        virtual ~Camera() { }

        // Wrappers
        void clear() { this->viewport->clear(); }
        void flush() { this->viewport->flush(); }
        TCODColor getCharForeground( int cx, int cy ) { return viewport->getCharForeground( cx, cy ); }
        TCODColor getCharBackground( int cx, int cy ) { return viewport->getCharBackground( cx, cy ); }
        void setCharForeground( int cx, int cy, const TCODColor *fg ) { viewport->setCharForeground( cx, cy, *fg ); }
        void setCharBackground( int cx, int cy, const TCODColor *bg ) { viewport->setCharBackground( cx, cy, *bg ); }

        Mouse getAbsoluteMouse( TCOD_mouse_t& relative_mouse ); // Mouse defined in engine.hpp
        void loadCamera( int width, int height, Actor *target );
        void recenter();
        void setChar( int cx, int cy, int code, const TCODColor *fg = nullptr, const TCODColor *bg = nullptr );
        void addExplosion( int x, int y );
        void addFooter( int x, int y, const char *text );

        virtual void update();
        virtual void render();
    };
};
