#pragma once
namespace cursed
{
    class Actor;
}

namespace cursed
{
    class AI
    {

        public:
        AI();
        virtual ~AI() { }

        virtual void update( Actor *owner ) = 0; // Pure virtual function
    };

    class PlayerAI : public AI
    {
        public:
        void update( Actor *owner );

        protected:
        bool moveOrAttack( Actor *owner, int target_x, int target_y );

    };

    class MonsterAI : public AI
    {
        public:
        void update( Actor *owner );

        protected:
        int move_count;
        bool moveOrAttack( Actor *owner, int target_x, int target_y );

    };
};
