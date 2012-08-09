// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

#include <vector>
#include <cstdlib>
#include <cmath>

inline double modulo(double a, double b)
{
    return a - floor(a / b) * b;
}

inline double wrapAngle(double a)
{
    return modulo(a, 2 * M_PI);
}

inline double sgn(double x)
{
    if(x < 0) return -1;
    if(x > 0) return 1;
    return 0;
}

inline bool near(double a, double b)
{
    return fabs(a - b) < 0.01;
}

inline bool thresh(double a, double b, double range)
{
    return fabs(a - b) < range;
}

inline int quadrant(double a)
{
    a = wrapAngle(a);
    if(near(a, 0)) return 0;
    if(near(a, M_PI / 2)) return 1;
    if(near(a, M_PI)) return 2;
    if(near(a, 3 * M_PI / 2)) return 3;
    if(near(a, 2 * M_PI)) return 0;
    
    if(a >= 0 && a < M_PI / 2) return 0;
    if(a >= M_PI / 2 && a < M_PI) return 1;
    if(a >= M_PI && a < 3 * M_PI / 2) return 2;
    if(a >= 3 * M_PI / 2 && a < 2 * M_PI) return 3;
    return 0;
}

inline bool isRightAngle(double a)
{
    return near(a, 0) || near(a, M_PI / 2) || near(a, M_PI) || near(a, 3 * M_PI / 2) || near(a, 2 * M_PI);
}

#ifdef _WIN32
#include <GL/glui.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#else
#include <glui.h>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#endif

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "color.h"
#include "transformation.h"
#include "texture.h"

#include "shape.h"
#include "cube.h"
#include "sphere.h"

#include "wall.h"
#include "map.h"
#include "bouncy.h"
#include "player.h"

#include "light.h"
