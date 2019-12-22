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
        char UP    = 'w';
        char DOWN  = 's';
        char LEFT  = 'a';
        char RIGHT = 'd';
    };
};
