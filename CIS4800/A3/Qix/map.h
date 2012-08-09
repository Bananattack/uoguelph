// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

namespace Qix
{
    class Map
    {
        public:
            enum MapTile
            {
                Empty = 0,
                Edge = 1,
                Solid = 2,
                InteriorEdge = 3,
                UnfinishedLine = 4,
                OutOfBounds = 5,
            };        
            
            
            static const int WORLD_START_X = -9;
            static const int WORLD_START_Y = -7;        
            static const int WORLD_WIDTH = 18;
            static const int WORLD_HEIGHT = 14;
            static const int MAP_WIDTH = 18 * 2;
            static const int MAP_HEIGHT = 14 * 2;
            static const double UNIT_WIDTH = 0.5;
            static const double UNIT_HEIGHT = 0.5;

            MapTile data[MAP_WIDTH * MAP_HEIGHT];
            MapTile temp[MAP_WIDTH * MAP_HEIGHT];
            std::vector<Wall> walls;
            Texture* edgeTexture;
            
            Map()
            {
            }
            
            void init(Texture* tex)
            {
                edgeTexture = tex;
                memset(data, 0, sizeof(data));
                
                walls.clear();
                bool edge;
                for(int j = 0; j < MAP_HEIGHT; j++)
                {
                    for(int i = 0; i < MAP_WIDTH; i++)
                    {
                        edge = (i == 0 || i == MAP_WIDTH - 1 || j == 0 || j == MAP_HEIGHT - 1);
                        walls.push_back(
                            Wall(
                                Cube(
                                    Transformation(
                                        Vector3(
                                            WORLD_START_X + i * UNIT_WIDTH,
                                            WORLD_START_Y + j * UNIT_HEIGHT,
                                            0
                                        ),
                                        Vector3(UNIT_WIDTH, UNIT_HEIGHT, 1),
                                        Vector3(0, 0, 0)
                                    ),
                                    edge ? edgeTexture: NULL
                                )
                            )
                        );
                        setTile(i, j, edge ? Edge : Empty);
                    }
                }
            }
            
            MapTile getTile(int x, int y)
            {
                if(x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
                    return OutOfBounds;
                return data[y * MAP_WIDTH + x]; 
            }            
            
            void setTile(int x, int y, MapTile tile)
            {
                setTile(y * MAP_WIDTH + x, tile);                
            }
            
            void setTile(int i, MapTile tile)
            {
                data[i] = tile;
                if(tile == Empty)
                {
                    walls[i].visible = false;
                }                     
                if(tile == Edge)
                {
                    if((i % MAP_WIDTH) == 0 || (i % MAP_WIDTH) == MAP_WIDTH - 1
                        || (i / MAP_WIDTH) == 0 || (i / MAP_WIDTH) == MAP_HEIGHT - 1)
                    {
                        walls[i].shape.ambient() = (Qix::Color) 0xffffffff;
                        walls[i].shape.texture = edgeTexture;
                        walls[i].visible = true;                    
                    }
                    else
                    {
                        walls[i].shape.ambient() = (Qix::Color) 0xf33cc66;
                        walls[i].shape.texture = NULL;
                        walls[i].visible = true;                        
                    }
                }
                if(tile == Solid)
                {
                    walls[i].shape.ambient() = (Qix::Color) 0xff227744;
                    walls[i].shape.texture = NULL;
                    walls[i].visible = true;
                }
             
                if(tile == UnfinishedLine)
                {
                    walls[i].shape.ambient() = (Qix::Color) 0xffcccc66;
                    walls[i].shape.texture = NULL;
                    walls[i].visible = true;
                }                
            }            
            
            void render()
            {
                for(size_t i = 0; i < walls.size(); i++)
                {
                    Wall& wall = walls[i];
                    wall.render();
                }
            }
            
            void replaceTile(MapTile find, MapTile replacement)
            {
                for(int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
                {
                    if(data[i] == find)
                    {
                        setTile(i, replacement);
                    }
                }
            }
            
            void store()
            {
                printf("STORING...");
                for(int j = MAP_HEIGHT - 1; j >= 0; j--)
                {
                    for(int i = 0; i < MAP_WIDTH; i++)
                    {
                        printf("%d", data[j * MAP_WIDTH + i]);
                    }
                    printf("\n");
                }            
                memcpy(temp, data, MAP_WIDTH * MAP_HEIGHT * sizeof(MapTile));
            }
            
            void recall()
            {
                printf("RECALLING...");            
                for(int j = MAP_HEIGHT - 1; j >= 0; j--)
                {
                    for(int i = 0; i < MAP_WIDTH; i++)
                    {
                        printf("%d", data[j * MAP_WIDTH + i]);
                    }
                    printf("\n");
                }
                memcpy(data, temp, MAP_WIDTH * MAP_HEIGHT * sizeof(MapTile));
                
                // Refresh walls to old data.
                for(int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
                {
                    setTile(i, data[i]);
                }
            }            
            
            bool fill(int x, int y, int bouncyX, int bouncyY)
            {
                //printf("%d, %d = %d\n", x, y, getTile(x, y));
                if(x == bouncyX && y == bouncyY)
                {
                    return false;
                }
                if(getTile(x, y) == Empty)
                {
                    setTile(x, y, Solid);
                    
                    bool b = fill(x - 1, y, bouncyX, bouncyY);
                    b &= fill(x + 1, y, bouncyX, bouncyY);
                    b &= fill(x, y - 1, bouncyX, bouncyY);
                    b &= fill(x, y + 1, bouncyX, bouncyY);
                    return b;
                }
                return true;
            }
    };
}