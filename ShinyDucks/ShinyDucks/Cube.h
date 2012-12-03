#pragma once
#include "Shape.h"

#define NUM_FACES 6


class Cube : public Shape
{
public:
	Cube(void);
	~Cube(void);
	virtual void draw();
};

