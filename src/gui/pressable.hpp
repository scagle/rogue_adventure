#pragma once

namespace rogue_adventure
{
    class GUI;
};

#include "../enums/menu_action.hpp"

#include <libtcod/libtcod.hpp>

namespace rogue_adventure
{
    class Pressable
    {
        private:        
        GUI *target = nullptr; 
        MenuAction action = MenuAction::NOTHING; 
        std::vector< GUI* > requirements;
        bool enabled = true; // If button is not disabled
        TCODColor selected_color = TCODColor::lightPurple;

        public:
        Pressable();
        virtual ~Pressable() { }

        virtual GUI* getTarget() { return this->target; } 
        virtual MenuAction getAction() { return this->action; } 
        virtual std::vector< GUI* >& getRequirements() { return this->requirements; } 
        virtual TCODColor getSelectedColor() { return TCODColor::lightPurple; }
        virtual void setTarget( GUI *target ) { this->target = target; } 
        virtual void setAction( MenuAction action ) { this->action = action; } 
        virtual void addRequirement( GUI* gui ) { this->requirements.push_back( gui ); } 

        virtual bool isPressable() { return this->enabled; } 

        void update( GUI *owner );
        void render( TCODConsole *console, GUI *owner );
    };
};
