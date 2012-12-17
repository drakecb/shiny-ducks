#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Source.h"
#include "Sphere.h"
#include "Plane.h"
#include "BMP_utils.h"
#include "SDL_utils.h"

#define AA_DEPTH 1
#define AA_THRESHOLD 0.1
#define AMBIENT_LIGHT 0.2
#define ACCURACY 0.000001
#define WIDTH 1920
#define HEIGHT 1080
#define DPI 72
#define N WIDTH * HEIGHT

#endif