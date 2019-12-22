#pragma once

namespace cursed
{
    class Actor;
}

#include <string>

namespace cursed
{
    class Destructible
    {
        public:
        float max_hp; // maximum health points
        float hp;     // health points
        float defense; // hit points deducted  
        bool corpse_visible; // corpse visibility (false in case of obstructions)
        std::string corpse_name; // name of visible corpse

        Destructible( float max_hp, float defense, bool corpse_visible, std::string corpse_name ); 
        virtual ~Destructible() { }

        float takeDamage( Actor *owner, float damage );
        float heal( float amount );
        virtual void die( Actor *owner );

        inline bool isDead() { return hp <= 0; }

    };

    class MonsterDestructible : public Destructible
    {
        public:
        MonsterDestructible( float max_hp, float defense, bool corpse_visible, 
            std::string corpse_name );
        void die( Actor *owner );
    };

    class PlayerDestructible : public Destructible
    {
        public:
        PlayerDestructible( float max_hp, float defense, bool corpse_visible, 
            std::string corpse_name );
        void die( Actor *owner );
    };
};
