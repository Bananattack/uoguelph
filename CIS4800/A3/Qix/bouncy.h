// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

namespace Qix
{
    class Bouncy
    {
        private:
            static const double SPEED = 0.1;
            static const double MOVEMENT_STEP = 0.01;
                      
        public:
            double speed;
            double angle;
            
            double bounceAngle;
            int bounceCount;
            
            Sphere shape;
            
        
            Bouncy()
            {
                speed = angle = 0;
                init();
            }
        
            Bouncy(Sphere sphere)
                : shape(sphere)
            {   
                speed = SPEED;
                angle = 85 * M_PI / 180; //M_PI / 2;
                init();
            }
            
            ~Bouncy()
            {
            
            }
            
            void init()
            {
                bounceCount = 0;
            }
            
            void update(std::vector<Wall>& walls)
            {
                for(double i = 0; i < speed; i += MOVEMENT_STEP)
                {                    
                    handleCollision(walls);
                 
                    Vector3& v = shape.transformation().translation;   
                    v.data[Vector3::X] += cos(angle) * MOVEMENT_STEP;
                    v.data[Vector3::Y] += sin(angle) * MOVEMENT_STEP;
                }
            }
            
            void render()
            {
                shape.render();
            }
            
            Vector2 getMapPosition()
            {
                Vector2 position = shape.transformation().translation.flatten();
                
                double x = floor(position.data[Vector2::X] + 0.5);
                double y = floor(position.data[Vector2::Y] + 0.5);
                
                x -= Map::WORLD_START_X;
                x += Map::UNIT_WIDTH / 2;
                y -= Map::WORLD_START_Y;
                y += Map::UNIT_HEIGHT / 2;
                x /= Map::UNIT_WIDTH;
                y /= Map::UNIT_HEIGHT;
                
                position.data[Vector2::X] = (int) x;
                position.data[Vector2::Y] = (int) y;
                return position;
            }
            
            void handleCollision(std::vector<Wall>& walls)
            {                
                Vector2 position = shape.transformation().translation.flatten();
                Vector2::Scalar radiusSquared = shape.transformation().scale.flatten().lengthSquared();
                
                for(unsigned int i = 0; i < walls.size(); i++)
                {
                    Wall& wall = walls[i];
                    if(!wall.visible)
                    {
                        continue;
                    }
                    
                    int q = quadrant(angle);
                    bool detect;                                   
                    for(int e = 0; e < 4; e++)
                    {
                        switch(q)
                        {
                            case 0:
                                detect = e == 2 || e == 3;
                                break;
                            case 1:
                                detect = e == 2 || e == 1;
                                break;
                            case 2:
                                detect = e == 0 || e == 1;
                                break;
                            case 3:
                                detect = e == 0 || e == 3;
                                break;
                        }
                        if(detect && wall.visible && wall.distanceSquared(e, position) <= radiusSquared)
                        {
                            Vector2 v = Vector2(cos(angle) * MOVEMENT_STEP, sin(angle) * MOVEMENT_STEP);
                            angle = wall.reflectionAngle(e, v.normalize());

                            //printf("%lf angle, q = %d, BOUNCE w = %u i = %d e= %d\n", angle * 180 / M_PI, q, i, e, bounceCount);
                            
                            //printf("%lf - %lf = %lf vs %lf\n", angle, modulo(bounceAngle + M_PI, 2 * M_PI), modulo(angle - bounceAngle, 2 * M_PI), M_PI);
                            if(bounceCount && near(wrapAngle(angle - bounceAngle), M_PI))
                            {
                                if(bounceCount == 2)
                                {
                                    angle += (1 - ((double) rand() / ((double) RAND_MAX + 1.0) * 2)) * M_PI / 6;
                                    angle += sgn(angle) * M_PI / 4;
                                    bounceCount = 0;
                                }
                            }
                            else
                            {
                                bounceCount = 0;
                            }
                            bounceAngle = angle;
                            bounceCount++;                            
                            
                            return;
                        }
                    }
                }
            }
    };
}
