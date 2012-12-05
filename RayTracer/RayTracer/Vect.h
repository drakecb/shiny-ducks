#ifndef _VECT_H
#define _VECT_H

#include "math.h"

class Vect {
	double x, y, z;

public:
	Vect();
	Vect(double, double, double);

	// methods
	double getVectX() { return x; }
	double getVectY() { return y; }
	double getVectZ() { return z; }

	double magnitude();
	Vect normalize();
	Vect negative();
	double dotProduct(Vect v);
	Vect crossProduct(Vect v);
	Vect vectAdd(Vect v);
	Vect vectMult(double scalar);
};

#endif