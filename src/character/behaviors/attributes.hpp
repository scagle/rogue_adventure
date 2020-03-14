#pragma once

namespace rogue_adventure
{
    class Attributes
    {
        public:
        int upgrade_points = 5;

        // Physical Attributes
        int health = 1;
        int strength = 0;
        int dexterity = 0;
        int magic = 0;

//      int unarmed = 0;
        int sword = 0;
//      int dagger = 0;
//      int bow = 0;
        int staff = 0;

        public:
        Attributes();
        virtual ~Attributes() { }
    };
};
