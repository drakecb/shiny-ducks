#include "Color.h"

Color::Color() {
	red = 0.5;
	green = 0.5;
	blue = 0.5;
}

Color::Color(double r, double g, double b, double s) {
	red = r;
	green = g;
	blue = b;
	reflect = s;
}

double Color::brightness() {
	return (red + green + blue) / 3.0;
}

Color Color::colorScalar(double scalar) {
	return Color (red * scalar, green * scalar, blue * scalar, reflect);
}

Color Color::colorAdd(Color color) {
	return Color (red + color.getColorRed(), green + color.getColorGreen(), blue + color.getColorBlue(), reflect);
}

Color Color::colorMultiply(Color color) {
	return Color (red * color.getColorRed(), green * color.getColorGreen(), blue * color.getColorBlue(), reflect);
}

Color Color::clip() {
	double allLight = red + green + blue;
	double excesslight = allLight - 3;
	if (excesslight > 0) {
		red = red + excesslight * (red / allLight);
		green = green + excesslight * (green / allLight);
		blue = blue + excesslight * (blue / allLight);
	}
	if (red > 1) { red = 1; }
	if (green > 1) { green = 1; }
	if (blue > 1) { blue = 1; }
	if (red < 0) { red = 0; }
	if (green < 0) { green = 0; }
	if (blue < 0) { blue = 0; }
	
	return Color (red, green, blue, reflect);
}