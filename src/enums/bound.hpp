#pragma once 

namespace rogue_adventure
{
    struct Bound
    {
        int x;
        int y;
        int w;
        int h;

        // Getting sick of converting Left Aligned to Center Aligned coordinates
        Bound getCentered()
        {
            return { x + w / 2, y, w, h };
        }
    };
};
