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

        int PICK_UP = 'g';
        int INVENTORY = 'i';
        int DROP = 'h';
    };
};
