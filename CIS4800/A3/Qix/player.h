// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

namespace Qix
{
    class Player
    {
        private:
            static const int StepDelay = 5;
            static const unsigned int FillColor = 0xff3366cc;
                      
        public:
            double angle;
            
            int tx, ty;
            int delay;
            Sphere shape;
            
            // Is moving along a path.
            bool moving;
            // Is building new walls.
            bool building;
            
            bool wallInProgress;
            int wallStartX, wallStartY;
            bool fillMe;
        
            Player()
            {
                init();
            }
        
            ~Player()
            {
            
            }            
            
            void init()
            {
                angle = 0;
                tx = ty = delay = 0,
                moving = false;
                building = false;
                fillMe = false;
                wallInProgress = false;                
                shape = Sphere(
                    Transformation(
                        Vector3(0, 0, 0.5),
                        Vector3(0.2, 0.2, 0.2),
                        Vector3(0, 0, 0)
                    ),
                    (Color) FillColor
                );
                Vector3& v = shape.transformation().translation;
                v.data[Vector3::X] = Map::WORLD_START_X + tx * Map::UNIT_WIDTH;
                v.data[Vector3::Y] = Map::WORLD_START_Y + ty * Map::UNIT_HEIGHT;                   
            }
            
            void update(Map& map)
            {
                const int X = Vector3::X;
                const int Y = Vector3::Y;

                if(delay > 0)
                {
                    delay--;
                    return;
                }
                if(moving)
                {
                    Vector3& v = shape.transformation().translation;
                    int dx = tx;
                    int dy = ty;
                    if(near(angle, 0)) dx++;
                    if(near(angle, M_PI / 2)) dy++;
                    if(near(angle, M_PI)) dx--;
                    if(near(angle, M_PI * 3 / 2)) dy--;

                    if(dx >= 0 && dx < Map::MAP_WIDTH && dy >= 0 && dy < Map::MAP_HEIGHT)
                    {
                        bool obstruct = true;
                        Map::MapTile t = map.getTile(dx, dy);
                        //printf("Wall (x = %d, y = %d, t = %d)\n", dx, dy, t);
                        if(building)
                        {
                            if(wallInProgress)
                            {
                                switch(t)
                                {
                                    case Map::Edge:
                                        wallInProgress = false;
                                        obstruct = false;
                                        fillMe = true;
                                        break;
                                    case Map::Empty:
                                        // Make sure adjacents (excepting previous point) are empty.
                                        if((tx == dx - 1 || map.getTile(dx - 1, dy) != Map::UnfinishedLine)
                                            && (tx == dx + 1 || map.getTile(dx + 1, dy) != Map::UnfinishedLine)
                                            && (ty == dy - 1 || map.getTile(dx, dy - 1) != Map::UnfinishedLine)
                                            && (ty == dy + 1 || map.getTile(dx, dy + 1) != Map::UnfinishedLine)
                                        )
                                        {
                                            obstruct = false;                                           
                                            map.setTile(dx, dy, Map::UnfinishedLine);
                                        }
                                        break;
                                    default:
                                        break;                                                                            
                                }                            
                            }
                            else
                            {
                                // Only move if building a new wall.
                                switch(t)
                                {
                                    case Map::Empty:
                                        // Make sure adjacents (excepting previous point) are empty.
                                        if((tx == dx - 1 || !map.getTile(dx - 1, dy))
                                            && (tx == dx + 1 || !map.getTile(dx + 1, dy))
                                            && (ty == dy - 1 || !map.getTile(dx, dy - 1))
                                            && (ty == dy + 1 || !map.getTile(dx, dy + 1))
                                        )
                                        {
                                            wallInProgress = true;
                                            wallStartX = dx;
                                            wallStartY = dy;
                                            obstruct = false;
                                            map.setTile(dx, dy, Map::UnfinishedLine);
                                        }
                                        break;   
                                    default:
                                        break;
                                }
                            }

                        }
                        else
                        {
                            if(map.getTile(tx, ty) != Map::UnfinishedLine)
                            {
                                if(t == Map::Edge)
                                {
                                    obstruct = false;
                                }
                            }
                        }
                        
                        if(!obstruct)
                        {
                            tx = dx;
                            ty = dy;
                            v.data[X] = Map::WORLD_START_X + dx * Map::UNIT_WIDTH;
                            v.data[Y] = Map::WORLD_START_Y + dy * Map::UNIT_HEIGHT;                        
                        }
                    }
                    delay = StepDelay;
                }
            }
            
            void render()
            {
                shape.render();
            }
    };
}
