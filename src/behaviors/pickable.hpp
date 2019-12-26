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
        Pickable() { }
        virtual ~Pickable() { }

        bool pick( Actor &owner, Actor &wearer );
        void drop( Actor &owner, Actor &wearer );
        virtual bool use( Actor &owner, Actor &wearer );
    };

    // Heal owner in some way ( health potion, food, etc )
    class Healer : public Pickable
    {
        public:
        float amount; // how much hp

        Healer( float amount ) : amount( amount ) { }
        bool use( Actor &owner, Actor &wearer );
    };

    // Damages monsters nearby player
    class LightningBolt : public Pickable
    {
        public:
        float range, damage;

        LightningBolt( float range, float damage ) : range( range ), damage( damage ) { }
        bool use( Actor &owner, Actor &wearer );
    };

    // Damages targetted monster
    class Fireball : public Pickable
    {
        public:
        float range, damage;

        Fireball( float range, float damage ) : range( range ), damage( damage ) { }
        bool use( Actor &owner, Actor &wearer );
    };

    // Confuses targeted monster
    class Confuser : public Pickable
    {
        public:
        int turns;
        float range;

        Confuser( int turns, float range ) : turns( turns ), range( range ) { }
        bool use( Actor &owner, Actor &wearer );
    };

};
