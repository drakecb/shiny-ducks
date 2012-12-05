#ifndef _PLANE_H
#define _PLANE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Plane : public Object {
	Vect normal;
	double distance;
	Color color;

public:
	Plane();
	Plane(Vect, double, Color);

	// methods
	Vect getPlaneNormal() { return normal; };
	double getPlaneDistance() { return distance; };
	virtual Color getColor() { return color; };

	virtual Vect getNormalAt(Vect point) { return normal; };
	virtual double findIntersection (Ray ray);
};

#endif