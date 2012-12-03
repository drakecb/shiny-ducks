#pragma once
#ifndef __POSITION_H
#define __POSITION_H

struct Position {
	float x, y, z;
};

struct Vector {
	float x, y, z;
};

struct Color {
	float r, g, b, a;
};

struct Vertex {
	Position position;
	Color diffuse, specular;
	float reflectivity;
};

struct Ray {
	Position origin;
	Vector direction;
};

#endif