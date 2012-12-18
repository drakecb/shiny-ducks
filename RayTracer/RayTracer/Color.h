#ifndef _COLOR_H
#define _COLOR_H

#include "math.h"

class Color {
	double red, green, blue, reflect;

public:
	Color();
	Color(double, double, double, double);

	// Methods
	double getColorRed() { return red; }
	double getColorGreen() { return green; }
	double getColorBlue() { return blue; }
	double getColorReflect() { return reflect; }

	void setColorRed(double redValue) { red = redValue; };
	void setColorGreen(double greenValue) { green = greenValue; };
	void setColorBlue(double blueValue) { blue = blueValue; };
	void setColorReflect(double reflectValue) { reflect = reflectValue; };

	// For shadows
	Color colorScalar(double scalar);
	Color colorAdd(Color color);
	Color colorMultiply(Color color);
	Color clip();
};

#endif