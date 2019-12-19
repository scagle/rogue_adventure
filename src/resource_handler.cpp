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
            maps.push_back( Map( Engine::getEngine(), 80, 50, tiles ) );

            // Add aggressive trolls to map
            TCODRandom *rng = TCODRandom::getInstance();
            for ( int i = 0; i < rng->getInt(5, 10); i++ )
            {
                Actor *actor = new Actor( Engine::getEngine(), 
                    rng->getInt(0, 79), rng->getInt(0, 49), 
                    'O', "troll", TCODColor::desaturatedGreen
                );
                actor->destructible = new MonsterDestructible( 16, 1, true, "troll's carcass" );
                actor->attacker = new Attacker( rng->getInt( 1, 4 ) );
                actor->ai = new MonsterAI();
                maps[maps.size()-1].addActor( actor );
            }

            // Add friendly npcs to map
            for ( int i = 0; i < rng->getInt(5, 10); i++ )
            {
                Actor *actor = new Actor( Engine::getEngine(), 
                    rng->getInt(0, 79), rng->getInt(0, 49), 
                    'O', "friendly dude", TCODColor::yellow
                );
                actor->destructible = new MonsterDestructible(16, 1, true, "dude's carcass");
                actor->attacker = nullptr;
                actor->ai = new MonsterAI();
                maps[maps.size()-1].addActor( actor );
            }
        }

        return true;
    }
};
