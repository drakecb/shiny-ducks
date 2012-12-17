#ifndef _COLOR_H
#define _COLOR_H

#include "math.h"

class Color {
	double red, green, blue, special;

public:
	Color();
	Color(double, double, double, double);

	// methods
	double getColorRed() { return red; }
	double getColorGreen() { return green; }
	double getColorBlue() { return blue; }
	double getColorSpecial() { return special; }

	void setColorRed(double redValue) { red = redValue; };
	void setColorGreen(double greenValue) { green = greenValue; };
	void setColorBlue(double blueValue) { blue = blueValue; };
	void setColorSpecial(double specialValue) { special = specialValue; };

	// for shadows
	double brightness();
	Color colorScalar(double scalar);
	Color colorAdd(Color color);
	Color colorMultiply(Color color);
	Color colorAverage(Color color);
	Color clip();
};

#endif