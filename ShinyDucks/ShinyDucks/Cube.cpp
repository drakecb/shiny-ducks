#include <cstdlib>
#include "Cube.h"


Vertex vertices[] = { {-1.0, -1.0, -1.0},
                      {1.0, -1.0, -1.0},
                      {1.0, 1.0, -1.0},
                         {-1.0, 1.0, -1.0},
                         {-1.0, -1.0, 1.0},
                         {1.0, -1.0, 1.0},
                         {1.0, 1.0, 1.0},
                         {-1.0, 1.0, 1.0}};

Vector normals[] = { { 0.0, 0.0, 1.0 },
{ 0.0, 1.0, 0.0 },
{-1.0, 0.0, 0.0 },
{ 1.0, 0.0, 0.0 },
{ 0.0, 0.0,-1.0 },
{ 0.0,-1.0, 0.0 } };

Color colors[] = {  {0.0, 0.0, 0.0, 1.0},
                       {1.0, 0.0, 0.0, 1.0},
                       {1.0, 1.0, 0.0, 1.0},
                       {0.0, 1.0, 0.0, 1.0},
                       {0.0, 0.0, 1.0, 1.0},
                       {1.0, 0.0, 1.0, 1.0},
                       {1.0, 1.0, 1.0, 1.0},
                       {0.0, 1.0, 1.0, 1.0}};

Cube::Cube(void)
{
	this->surfaces = (Surface *) malloc(sizeof(Surface)*6);
	Vertex tempVertices[4];

	tempVertices[0] = vertices[1];
	tempVertices[1] = vertices[0];
	tempVertices[2] = vertices[3];
	tempVertices[3] = vertices[2];
	(this->surfaces)[0] = Surface(tempVertices, 4, normals[0]);

	tempVertices[0] = vertices[3];
	tempVertices[1] = vertices[7];
	tempVertices[2] = vertices[6];
	tempVertices[3] = vertices[2];
	this->surfaces[1] = Surface(tempVertices, 4, normals[1]);

	tempVertices[0] = vertices[7];
	tempVertices[1] = vertices[3];
	tempVertices[2] = vertices[0];
	tempVertices[3] = vertices[4];
	this->surfaces[2] = Surface(tempVertices, 4, normals[2]);

	tempVertices[0] = vertices[2];
	tempVertices[1] = vertices[6];
	tempVertices[2] = vertices[5];
	tempVertices[3] = vertices[1];
	this->surfaces[3] = Surface(tempVertices, 4, normals[3]);

	tempVertices[0] = vertices[4];
	tempVertices[1] = vertices[5];
	tempVertices[2] = vertices[6];
	tempVertices[3] = vertices[7];
	this->surfaces[4] = Surface(tempVertices, 4, normals[4]);

	tempVertices[0] = vertices[5];
	tempVertices[1] = vertices[4];
	tempVertices[2] = vertices[0];
	tempVertices[3] = vertices[1];
	this->surfaces[5] = Surface(tempVertices, 4, normals[5]);
}


Cube::~Cube(void)
{
	// dealloc the surfaces
}

void Cube::draw()
{
	for(int i=0; i<NUM_FACES; i++) 
	{
		Surface surface = this->surfaces[i];

		Vertex* vertices = surface.getVertices();
		Vector normal = surface.getNormal();
		int numVertices = 4;

		glBegin(GL_POLYGON);
		for(int j=0; j<numVertices; j++) 
		{
			Vertex vertex = vertices[j];
			glColor3f(1.0, 0.25 * j, 0.0);
			glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
		}
		glEnd();
	}
}
