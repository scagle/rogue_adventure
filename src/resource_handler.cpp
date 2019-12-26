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
                actor->attacker = std::make_unique< Attacker >( rng->getInt( 4, 4 ) );
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
                actor->attacker = std::make_unique< Attacker >( 0 );
                actor->ai = std::make_unique< MonsterAI >();

                // Transfer ownership to current map
                maps[maps.size()-1]->addActor( std::move( actor ) );
            }
            // Add Health items to map
            for ( int i = 0; i < rng->getInt(5, 50); i++ )
            {
                int type = rng->getInt( 0, 100 );
                if ( type < 20 )
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
                else if ( type < 40 )
                {
                    // Create LightningBolt
                    std::unique_ptr< Actor > lbolt = std::make_unique< Actor >( Engine::getEngine(),
                        rng->getInt(0, 79), rng->getInt(0, 49),
                        '#', "Scroll of Lightning Bolt", TCODColor::lightYellow );
                    lbolt->blocks = false;
                    lbolt->pickable = std::make_unique< LightningBolt >( 5, 20 );

                    // Transfer ownership to current map
                    maps[maps.size()-1]->addItem( std::move( lbolt ) );
                }
                else if ( type < 60 )
                {
                    // Create Fireball
                    std::unique_ptr< Actor > fball = std::make_unique< Actor >( Engine::getEngine(),
                        rng->getInt(0, 79), rng->getInt(0, 49),
                        '#', "Scroll of Fireball", TCODColor::orange );
                    fball->blocks = false;
                    fball->pickable = std::make_unique< Fireball >( 5, 5 );

                    // Transfer ownership to current map
                    maps[maps.size()-1]->addItem( std::move( fball ) );
                }
                else 
                {
                    // Create Confuser
                    std::unique_ptr< Actor > confr = std::make_unique< Actor >( Engine::getEngine(),
                        rng->getInt(0, 79), rng->getInt(0, 49),
                        '#', "Scroll of Confusion", TCODColor::lightBlue );
                    confr->blocks = false;
                    confr->pickable = std::make_unique< Confuser >( 5, 5 );

                    // Transfer ownership to current map
                    maps[maps.size()-1]->addItem( std::move( confr ) );
                }
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
