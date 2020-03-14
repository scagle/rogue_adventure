#pragma once

namespace rogue_adventure
{
    class Actor;
};

namespace rogue_adventure
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
