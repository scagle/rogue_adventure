#pragma once

namespace cursed
{
    class Actor;
};

namespace cursed
{
    class Pickable
    {
        public:
        Pickable();
        virtual ~Pickable() { }

        bool pick( Actor &owner, Actor &wearer );
        virtual bool use( Actor &owner, Actor &wearer );
    };

    // Heal owner in some way ( health potion, food, etc )
    class Healer : public Pickable
    {
        public:
        float amount; // how much hp

        Healer( float amount );
        bool use( Actor &owner, Actor &wearer );
    };
};
