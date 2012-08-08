// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#define ABS(a) (((a) < 0) ? (-a) : (a))

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <GL/glui.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#define M_PI 3.141592653589793238462643
#else
#include <glui.h>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#endif

inline bool near(double a, double b)
{
    return fabs(a - b) < 0.01;
}

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "color.h"
#include "transformation.h"
#include "light.h"
#include "face.h"
#include "vertex.h"
#include "polygon.h"