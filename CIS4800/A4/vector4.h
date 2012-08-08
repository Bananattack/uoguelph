// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

struct Vector4
{
    enum Component
    {
        X = 0,
        Y = 1,
        Z = 2,
        W = 3,
    };

    GLfloat data[4];
    
    Vector4()
    {
        data[X] = data[Y] = data[Z] = data[W] = 0;
    }

    Vector4(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        data[X] = x;
        data[Y] = y;
        data[Z] = z;
        data[W] = w;
    }
};