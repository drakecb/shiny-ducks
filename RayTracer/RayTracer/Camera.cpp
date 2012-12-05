#include "Camera.h"

Camera::Camera() {
	campos = Vect(0, 0, 0);
	camdir = Vect(1, 0, 0);
	camright = Vect(0, 0, 0);
	camdown= Vect(0, 0, 0);
}

Camera::Camera(Vect pos, Vect dir, Vect right, Vect down) {
	campos = pos;
	camdir = dir;
	camright = right;
	camdown = down;
}