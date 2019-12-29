#pragma once

namespace cursed
{
    class Actor;
};

namespace cursed
{
    class Attacker
    {
        float power;

        public:
        Attacker( float power );
        virtual ~Attacker() { }
        
        void attack( Actor &owner, Actor &target );
    };
};
