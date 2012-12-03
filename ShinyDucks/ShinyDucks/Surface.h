#pragma once

#include "Structs.h"

class Surface
{
public:
	Surface(void);
	Surface(Vertex *vertices, int numVertices, Vector normal);
	~Surface(void);
	Vertex* getVertices() { return vertices; };
	Vector getNormal() { return normal; };
private:
	Vertex *vertices;
	Vector normal;
};

