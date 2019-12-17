#include "resource_handler.hpp"

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
            maps.push_back( Map( 80, 50, tiles ) );
            maps[maps.size()-1].addActor(new Actor(20, 25, '!', TCODColor::red ) );
        }

        return true;
    }
};
