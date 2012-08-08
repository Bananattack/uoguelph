// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

namespace Qix
{
    class Wall
    {
        public:
            Cube shape;
            Vector2 points[4];
            int edgePoints[4][2];
            Vector2 edgeNormals[4];
            
            Wall(Cube cube)
                : shape(cube)
            {   
                Vector2 wallPosition = shape.transformation().translation.flatten();
                Vector2 wallSize = shape.transformation().scale.flatten();
                
                points[0] = Vector2(
                                wallPosition.data[Vector2::X] - wallSize.data[Vector2::X] / 2,
                                wallPosition.data[Vector2::Y] + wallSize.data[Vector2::Y] / 2
                            );
                points[1] = Vector2(
                                wallPosition.data[Vector2::X] + wallSize.data[Vector2::X] / 2,
                                wallPosition.data[Vector2::Y] + wallSize.data[Vector2::Y] / 2
                            );
                points[2] = Vector2(
                                wallPosition.data[Vector2::X] + wallSize.data[Vector2::X] / 2,
                                wallPosition.data[Vector2::Y] - wallSize.data[Vector2::Y] / 2
                            );      
                points[3] = Vector2(
                                wallPosition.data[Vector2::X] - wallSize.data[Vector2::X] / 2,
                                wallPosition.data[Vector2::Y] - wallSize.data[Vector2::Y] / 2
                            );
            
                edgePoints[0][0] = 0;
                edgePoints[0][1] = 1;
                edgeNormals[0] = Vector2(0, 1);
                                
                edgePoints[1][0] = 1;
                edgePoints[1][1] = 2;
                edgeNormals[1] = Vector2(1, 0);        
                
                edgePoints[2][0] = 2;
                edgePoints[2][1] = 3;
                edgeNormals[2] = Vector2(0, -1);
                                
                edgePoints[3][0] = 3;
                edgePoints[3][1] = 0;
                edgeNormals[3] = Vector2(-1, 0);
            }
            
            ~Wall()
            {
            
            }
            
            void render()
            {
                shape.render();
            }
            
            Vector2::Scalar distanceSquared(int e, Vector2 point)
            {
                return point.distanceSquaredFromLine(points[edgePoints[e][0]], points[edgePoints[e][1]]);
            }
            
            Vector2::Scalar reflectionAngle(int e, Vector2 v)
            {
                //return v.subtract(edgeNormals[e].scale(2 * edgeNormals[e].dot(v))).direction();
                return 90 - v.subtract(edgeNormals[e].scale(2 * edgeNormals[e].dot(v))).direction();
            }
    };
}