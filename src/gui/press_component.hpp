#pragma once

namespace cursed
{
    class GUI;
};

#include "../enums/menu_action.hpp"

namespace cursed
{
    class PressComponent
    {
        public:        
        GUI *target = nullptr; // 
        MenuAction action; // Action function pointer

        public:
        PressComponent( GUI *target, MenuAction action );
        virtual ~PressComponent() { }
    };
};
