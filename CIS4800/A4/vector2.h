// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once


struct Vector2
{
    enum Component
    {
        X = 0,
        Y = 1,
    };

    GLfloat data[2];
    
    Vector2()
    {
        data[X] = data[Y] = 0;
    }

    Vector2(GLfloat x, GLfloat y)
    {
        data[X] = x;
        data[Y] = y;
    }
};
