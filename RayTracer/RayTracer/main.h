#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Source.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"

#define AA_DEPTH 1
#define AA_THRESHOLD 0.1
#define AMBIENT_LIGHT 0.2
#define ACCURACY 0.000001
#define WIDTH 640
#define HEIGHT 480
#define DPI 300
#define N WIDTH * HEIGHT

#endif