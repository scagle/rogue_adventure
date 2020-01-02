#include "world.hpp"

#include "zone.hpp"

#include "../enums/entry_point.hpp"

namespace cursed
{
    // Static Declaration
    Zone* World::current_zone = nullptr;
    int World::current_index = 0;
    bool World::in_dungeon = true;

    // Constructors
    World::World()
    {
        
    }

    // Methods
    void World::createWorld( int seed )
    {
        for ( int i = 0; i < zones.size(); i++ )
        {
            this->zones[i] = std::move( generator.createRandomZone( seed + i ) );
        }
        current_zone = zones[0].get();
    }

    bool World::switchTo( int tile_x, int tile_y, int *player_x, int *player_y, EntryPoint entry )
    {
        if ( !( tile_x >= 0 && tile_y >= 0 && tile_x < width && tile_y < height ) )
        {
            return false;
        }

        int next_index = tile_x + width * tile_y;
        Zone* next_zone = zones[ next_index ].get();

        switch ( entry )
        {
            case CENTER:
            {
                *player_x = next_zone->getWidth() / 2;    
                *player_y = next_zone->getHeight() / 2;
                break;
            }

            case EDGE:
            {
                int old_tile_x = current_index % width;
                int old_tile_y = current_index / width;
                int tile_dx = tile_x - old_tile_x; // - means left, + means right
                int tile_dy = old_tile_y - tile_y; // - means up,   + means down
                if ( tile_dx > 0 ) // If going right-wards
                {
                    *player_x = 0;
                }
                if ( tile_dy > 0 ) // If going down-wards
                {
                    *player_y = next_zone->getHeight() - 1;
                }
                if ( tile_dx < 0 ) // If going left-wards
                {
                    *player_x = next_zone->getWidth() - 1;
                }
                if ( tile_dy < 0 ) // If going up-wards
                {
                    *player_y = 0;
                }
                break;
            }

            default: // Keep player where they currently are
            {
                break;
            }
        }

        this->current_index = next_index;
        this->current_zone = next_zone;

        return true;
    }

};

