#pragma once

namespace cursed
{
    class Actor;
}

#include <libtcod/libtcod.hpp>
#include <memory>

namespace cursed
{
    // Constants
    const int INVENTORY_WIDTH = 50;     // width of console
    const int INVENTORY_HEIGHT = 50;    // height of console

    class AI
    {

        public:
        AI();
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

    class MonsterAI : public AI
    {
        public:
        void update( Actor &owner );

        protected:
        int move_count;
        bool moveOrAttack( Actor &owner, int target_x, int target_y );

    };
};
