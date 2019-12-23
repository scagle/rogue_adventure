#include "resource_handler.hpp"

#include "datatypes/tile.hpp"
#include "engine.hpp"

#include <fstream>
#include <sstream>
#include <glob.h>

namespace cursed
{
    bool ResourceHandler::loadResources()
    {
        if ( !loadMaps() )
        {
            printf("*** WARNING: Could not load maps (resource_handler.cpp)\n");
            return false;
        }

        initialized = true;

        return true;
    }

    bool ResourceHandler::loadMaps()
    {
        std::vector< std::string > paths;
        glob_t globbuf;
        glob("../assets/maps/*", GLOB_TILDE, NULL, &globbuf);
        for ( int i = 0; i < globbuf.gl_pathc; i++ )
        {
            paths.push_back( globbuf.gl_pathv[i] );
        }
        globfree( &globbuf );

        for ( auto&& path : paths )
        {
            std::ifstream file_stream(path);
            if ( !file_stream )
            {
                printf("*** WARNING: Can not open '%s'! (resource_handler.cpp)\n", path.c_str());
                continue;
            }

            Tile tiles[50 * 80];
            std::string line;
            int y = 0;
            while ( std::getline( file_stream, line ) )
            {
                std::istringstream iss( line );
                int x = 0;
                const int length = line.length();
                for ( auto&& ch : line )
                {
                    tiles[x + y*length] = Tile( ch );
                    x++;
                }
                y++;
            }
            printf("Successfully loaded '%s'\n", path.c_str());
            maps.push_back( std::make_unique< Map >( Engine::getEngine(), 80, 50, tiles ) );

            // Add aggressive trolls to map
            TCODRandom *rng = TCODRandom::getInstance();
            for ( int i = 0; i < rng->getInt(5, 10); i++ )
            {
                // Create troll as unique_ptr, who's ownership will be transferred to map
                std::unique_ptr< Actor > actor = std::make_unique< Actor >( Engine::getEngine(), 
                    rng->getInt(0, 79), rng->getInt(0, 49), 
                    'O', "troll", TCODColor::desaturatedGreen
                );
                actor->destructible = std::make_unique< Destructible >( 16, 1, true, 
                    "troll's carcass" );
                actor->attacker = std::make_unique< Attacker >( rng->getInt( 1, 4 ) );
                actor->ai = std::make_unique< MonsterAI >();

                // Transfer ownership to current map
                maps[maps.size()-1]->addActor( std::move( actor ) );
            }

            // Add friendly npcs to map
            for ( int i = 0; i < rng->getInt(5, 10); i++ )
            {
                // Create npc as unique_ptr, who's ownership will be transferred to map
                std::unique_ptr< Actor > actor = std::make_unique< Actor >( Engine::getEngine(), 
                    rng->getInt(0, 79), rng->getInt(0, 49), 
                    'O', "friendly dude", TCODColor::yellow
                );
                actor->destructible = std::make_unique< MonsterDestructible >( 16, 0, true, 
                    "dude's carcass" );
                actor->attacker = nullptr;
                actor->ai = std::make_unique< MonsterAI >();

                // Transfer ownership to current map
                maps[maps.size()-1]->addActor( std::move( actor ) );
            }
            // Add Health items to map
            for ( int i = 0; i < rng->getInt(5, 10); i++ )
            {
                // Create potion
                std::unique_ptr< Actor > potion = std::make_unique< Actor >( Engine::getEngine(),
                    rng->getInt(0, 79), rng->getInt(0, 49),
                    '!', "health potion", TCODColor::pink );
                potion->blocks = false;
                potion->pickable = std::make_unique< Healer >( 4 );

                // Transfer ownership to current map
                maps[maps.size()-1]->addItem( std::move( potion ) );
            }
        }

        return true;
    }

    Map* ResourceHandler::getMap( int index ) 
    { 
        if ( initialized == false || maps.size() <= index )
        {
            printf("*** WARNING: ResourceHandler is not initialized! (resource_handler.cpp)\n");
            return nullptr;
        }
        return this->maps[index].get(); 
    }
};
