#include "Sphere.h"

Sphere::Sphere() {
	center = Vect (0, 0, 0);
	radius = 1.0;
	color = Color (0.5, 0.5, 0.5, 0);
}

Sphere::Sphere(Vect centerValue, double radiusValue, Color colorValue) {
	center = centerValue;
	radius = radiusValue;
	color = colorValue;
}

Vect Sphere::getNormalAt(Vect point) {
	Vect normalVect = point.vectAdd(center.negative()).normalize();
	return normalVect;
}

double Sphere::findIntersection(Ray ray) {
	Vect rayOrigin = ray.getRayOrigin();
	Vect rayDirection = ray.getRayDirection();
	Vect sphereCenter = center;

	double a = rayDirection.dotProduct(rayDirection); // equation a = v^2
	double b = rayDirection.dotProduct(rayOrigin.vectAdd(sphereCenter.negative()).vectMult(2.0));
	Vect sub = rayOrigin.vectAdd(sphereCenter.negative());
	double c = sub.dotProduct(sub) - pow(radius, 2);

	double discriminant = b * b - 4 * c;
	if (discriminant > 0) {
		// the ray intersects the sphere

		// the first root
		double rootOne = ((-1 * b - sqrt(discriminant)) / 2) - 0.000001;

		if (rootOne > 0) {
			// the first root is the smallest positive root
			return rootOne;
		} else {
			// the second root is the smallest positive root
			double rootTwo = ((sqrt(discriminant) - b) / 2) - 0.000001;
			return rootTwo;
		}
	} else {
		// the ray missed the sphere
		return -1;
	}
}