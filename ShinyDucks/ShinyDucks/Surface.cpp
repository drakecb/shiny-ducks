#include "Surface.h"
#include <cstdlib>
#include <cstring>



Surface::Surface(void)
{
}

Surface::Surface(Vertex *verticers, int numVertices, Vector normal)
{
	this->vertices = (Vertex *)malloc( numVertices * sizeof(Vertex) );
	memcpy(this->vertices, verticers, numVertices * sizeof(Vertex));
	this->normal = normal;
}


Surface::~Surface(void)
{
}
