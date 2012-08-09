// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

#include <cmath>

namespace Qix
{
    struct Vector2
    {
        typedef GLfloat Scalar; 
        enum Component
        {
            X = 0,
            Y = 1,
        };

		Scalar data[2];
        
		Vector2()
		{
			data[X] = data[Y] = 0;
		}

		Vector2(Scalar x, Scalar y)
		{
			data[X] = x;
			data[Y] = y;
		}
      
        Scalar lengthSquared()
        {
            return data[X] * data[X] + data[Y] * data[Y];
        }
          
        Scalar length()
        {
            return sqrt(lengthSquared());
        }
        
        Scalar direction()
        {
            return atan2(data[Y], data[X]);
        }
        
        Scalar dot(Vector2& v)
        {
            return data[X] * v.data[X] + data[Y] * v.data[Y];
        }
        
        Vector2 scale(Scalar a)
        {
            return Vector2(data[X] * a, data[Y] * a);
        }
        
        Vector2 add(Vector2 v)
        {
            return Vector2(data[X] + v.data[X], data[Y] + v.data[Y]);
        }
        
        Vector2 subtract(Vector2 v)
        {
            return Vector2(data[X] - v.data[X], data[Y] - v.data[Y]);
        }

        Scalar distanceSquaredFromLine(Vector2& pointA, Vector2& pointB)
        {
            Vector2 lineSegment = pointB.subtract(pointA);
            Vector2 dispFromPointA = subtract(pointA);
            
            Scalar projScale = dispFromPointA.dot(lineSegment) / lineSegment.lengthSquared();
            if(projScale < 0)
            {
                projScale = 0;
            }
            else if(projScale > 1)
            {
                projScale = 1;
            }
            
            Vector2 closestPoint = pointA.add(lineSegment.scale(projScale));
            Vector2 dispFromClosestPoint = subtract(closestPoint);
            return dispFromClosestPoint.lengthSquared();
        }
        
        Vector2 normalize()
        {
            return scale(1 / length());
        }
    };
}