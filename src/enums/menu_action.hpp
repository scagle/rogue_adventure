#pragma once

namespace rogue_adventure
{
    enum class MenuAction
    {
        NOTHING,     // Do nothing

        EXIT,        // Exit game
        RESUME,      // Resume game

        PUSH,        // Push a GUI to the gui_stack (descend down hierarchy)
        POP,         // Pop a GUI from the gui_stack (ascend up hierarchy)
        FOCUS_INPUT, // Focus specific input above all else

        MAIN_MENU,   // Switch to Main Menu

        SAVE,        // Save Game at Slot #
        LOAD,        // Load Game at Slot #
    };
};
