#include "camera.hpp"
#include "engine.hpp"

namespace cursed
{
    // Static Declaration

    // Constructors
    Camera::Camera()
    {

    }

    // Methods
    Mouse Camera::getAbsoluteMouse( TCOD_mouse_t& relative_mouse ) // Mouse defined in engine.hpp
    {
        return { x + relative_mouse.cx, y + relative_mouse.cy };
    }

    void Camera::loadCamera( int width, int height, Actor *target )
    {
        this->width = width;
        this->height = height;
        this->target = target;
        this->viewport = std::make_unique< TCODConsole >( width, height );
        recenter();
    }
    void Camera::recenter()
    {
        int center_x = target->x;
        int center_y = target->y;
        this->x = center_x - width / 2;
        this->y = center_y - height / 2;
    }

    void Camera::setChar( int cx, int cy, int code, const TCODColor *fg, const TCODColor *bg )
    {
        if ( ! viewport )
        {
            printf("*** WARNING: Camera not initialized (camera.cpp)\n");
            return;
        }
        // Check if within visibile boundaries
        if ( cx > x && cx < x + width && cy > y && cy < y + height ) 
        {
            int relative_x = cx - this->x;
            int relative_y = cy - this->y;
            viewport->setChar( relative_x, relative_y, code );
            if ( fg )
            {
                viewport->setCharForeground( relative_x, relative_y, *fg );
            }
            if ( bg )
            {
                viewport->setCharBackground( relative_x, relative_y, *bg );
            }
        }
    }

    void Camera::addExplosion( int x, int y )
    {

    }

    void Camera::addFooter( int x, int y, const char *text )
    {

    }

    void Camera::update()
    {
        recenter();
    }

    void Camera::render()
    {
        if ( ! viewport )
        {
            printf("*** WARNING: Camera not initialized (camera.cpp)\n");
            return;
        }

        // blit the camera to the root console
        TCODConsole::blit( viewport.get(), 0, 0, width, height, TCODConsole::root, 0, 0 );
    }
};
