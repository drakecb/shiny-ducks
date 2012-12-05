#ifndef _SPHERE_H
#define _SPHERE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Sphere : public Object {
	Vect center;
	double radius;
	Color color;

public:
	Sphere();
	Sphere(Vect, double, Color);

	// methods
	Vect getSphereCenter() { return center; };
	double getSphereRadius() { return radius; };
	virtual Color getColor() { return color; };

	virtual Vect getNormalAt(Vect point);
	virtual double findIntersection(Ray ray);
};

#endif