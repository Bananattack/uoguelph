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
            static const double MOVEMENT_STEP = 0.0001;
        
            static double modulo(double a, double b)
            {
                return a - floor(a / b) * b;
            }
            
            static double sgn(double x)
            {
                if(x < 0) return -1;
                if(x > 0) return 1;
                return 0;
            }
            
            static bool near(double a, double b)
            {
                return fabs(a - b) < 0.01;
            }        
        
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
                speed = 0.1;
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
            
            void handleCollision(std::vector<Wall>& walls)
            {                
                Vector2 position = shape.transformation().translation.flatten();
                Vector2::Scalar radiusSquared = shape.transformation().scale.flatten().lengthSquared();
                
                for(unsigned int i = 0; i < walls.size(); i++)
                {
                    Wall& wall = walls[i];
                    for(int e = 0; e < 4; e++)
                    {
                        if(wall.distanceSquared(e, position) <= radiusSquared)
                        {
                            Vector2 v = Vector2(cos(angle) * MOVEMENT_STEP, sin(angle) * MOVEMENT_STEP);
                            angle = wall.reflectionAngle(e, v.normalize());

                            //printf("%lf angle BOUNCE %u %d %d\n", angle * 180 / M_PI, i, e, bounceCount);
                            
                            //printf("%lf - %lf = %lf vs %lf\n", angle, modulo(bounceAngle + M_PI, 2 * M_PI), modulo(angle - bounceAngle, 2 * M_PI), M_PI);
                            if(bounceCount && near(modulo(angle - bounceAngle, 2 * M_PI), M_PI))
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