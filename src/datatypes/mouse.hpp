#pragma once

namespace cursed
{
    // Absolute Mouse position
    struct Mouse
    {
        int x; // Absolute X value grabbed from highly relative Camera X value
        int y; // Absolute Y value grabbed from highly relative Camera Y value
    };
};
