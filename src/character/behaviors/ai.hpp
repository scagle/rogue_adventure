#pragma once

namespace rogue_adventure
{
    class Actor;
}

#include <libtcod/libtcod.hpp>
#include <memory>

namespace rogue_adventure
{
    // Constants
    const int INVENTORY_WIDTH = 50;     // width of console
    const int INVENTORY_HEIGHT = 50;    // height of console

    class AI
    {

        public:
        AI() { }
        virtual ~AI() { }

        bool pickUpItem( Actor& owner );
        virtual void update( Actor &owner ) = 0;
    };

    class PlayerAI : public AI
    {
        static TCODConsole inv_console;
        public:
        void update( Actor &owner );

        protected:
        bool moveOrAttack( Actor &owner, int target_x, int target_y );
        Actor* chooseFromInventory( Actor &owner ); // Can return nullptr

    };

    class FollowerAI : public AI
    {
        public:
        void update( Actor &owner );

        protected:
        int move_count;
        bool moveOrAttack( Actor &owner, int target_x, int target_y );

    };


    class MonsterAI : public AI
    {
        public:
        void update( Actor &owner );

        protected:
        int move_count;
        bool moveOrAttack( Actor &owner, int target_x, int target_y );

    };

    class ConfusedMonsterAI : public AI
    {
        protected:
        int turns;  // Number of turns
        std::unique_ptr< AI > old_ai; // Old AI that will be brought back after confusion ends

        public:
        ConfusedMonsterAI( int turns, std::unique_ptr< AI > old_ai );
        ~ConfusedMonsterAI() { }
        void update( Actor &owner );
        void restore( Actor &owner );
    };
};
