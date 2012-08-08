// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

namespace Qix
{
    struct Vector3
    {
        enum Component
        {
            X = 0,
            Y = 1,
			Z = 2,
        };

		GLfloat data[3];
        
		Vector3()
		{
			data[X] = data[Y] = data[Z] = 0;
		}

		Vector3(GLfloat x, GLfloat y, GLfloat z)
		{
			data[X] = x;
			data[Y] = y;
			data[Z] = z;
		}
    };
}