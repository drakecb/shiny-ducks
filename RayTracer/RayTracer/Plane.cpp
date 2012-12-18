#include "Plane.h"

Plane::Plane() {
	normal = Vect (1, 0, 0);
	distance = 0;
	color = Color (0.5, 0.5, 0.5, 0);
}

Plane::Plane(Vect normalValue, double distanceValue, Color colorValue) {
	normal = normalValue;
	distance = distanceValue;
	color = colorValue;
}

double Plane::findIntersection(Ray ray) {
	Vect rayDirection = ray.getRayDirection();

	double l = rayDirection.dotProduct(normal);

	if (l == 0) {
		// Ray is parallel to the plane
		return -1;
	} else {
		// Line-Plane Intersection -> d = ((p0 - l0) . n) / l . n
		double d = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
		return -1.0 * d / l;
	}
}