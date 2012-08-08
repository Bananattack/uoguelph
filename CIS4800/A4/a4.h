// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

#include <cstdlib>
#include <cmath>
#include <iostream>

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
#include "light.h"
#include "heightmap.h"