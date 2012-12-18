#ifndef _CAMERA_H
#define _CAMERA_H

#include "Vect.h"

class Camera {
	Vect camPos, camDir, camRight, camDown;

public:
	Camera();
	Camera(Vect, Vect, Vect, Vect);

	// methods
	Vect getCameraPosition() { return camPos; }
	Vect getCameraDirection() { return camDir; }
	Vect getCameraRight() { return camRight; }
	Vect getCameraDown() { return camDown; }
};

#endif