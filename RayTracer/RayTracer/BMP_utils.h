#ifndef _BMP_UTILS_H
#define _BMP_UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include "math.h"


struct RGBType {
	double r, g, b;
};

void savebmp(const char *fileName, int w, int h, int dpi, RGBType *data);

#endif