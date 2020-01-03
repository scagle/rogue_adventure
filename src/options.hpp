#pragma once

namespace cursed
{
    struct Options
    {        
        static Options& getOptions()
        {
            static Options instance; // Guaranteed to be destroyed
                               // Instantiated on first use
            return instance;
        }
        Options() {}  
        Options( Options const& ) = delete;
        void operator=( Options const& ) = delete;

        // Movement Keys  
        int UP    = 'w';
        int DOWN  = 's';
        int LEFT  = 'a';
        int RIGHT = 'd';
        int MENU_UP    = 'w';
        int MENU_DOWN  = 's';
        int MENU_LEFT  = 'a';
        int MENU_RIGHT = 'd';

        // Dungeon Keys
        int PICK_UP     = 'g';
        int INVENTORY   = 'i';
        int DROP        = 'z';

        // Temporary Keys
        int MAP_UP      = 'k';
        int MAP_DOWN    = 'j';
        int MAP_LEFT    = 'h';
        int MAP_RIGHT   = 'l';
        int MAP_SWITCH  = 'n';

        // Special Keys
        TCOD_keycode_t MENU = TCODK_ESCAPE;
        TCOD_keycode_t MENU_SELECT = TCODK_ENTER;
    };
};
