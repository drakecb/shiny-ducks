#pragma once
#include "Surface.h"
#include <gl/glut.h>

class Shape
{
public:
	Surface *surfaces;
	Shape(void);
	~Shape(void);
	virtual void draw() = 0;
};

