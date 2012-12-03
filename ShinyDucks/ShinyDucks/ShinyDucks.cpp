#include <iostream>
#include <gl/glut.h>
#include <vector>
#include "Cube.h"

#define WIDTH 800
#define HEIGHT 600

GLfloat vertices[][3] = {{-1.0, -1.0, -1.0},
                         { 1.0, -1.0, -1.0},
                         { 1.0,  1.0, -1.0},
                         {-1.0,  1.0, -1.0},
                         {-1.0, -1.0,  1.0},
                         { 1.0, -1.0,  1.0},
                         { 1.0,  1.0,  1.0},
                         {-1.0,  1.0,  1.0} };

GLfloat colors[][3] = {{0.0, 0.0, 0.0},
                       {1.0, 0.0, 0.0},
                       {1.0, 1.0, 0.0},
                       {0.0, 1.0, 0.0},
                       {0.0, 0.0, 1.0},
                       {1.0, 0.0, 1.0},
                       {1.0, 1.0, 1.0},
                       {0.0, 1.0, 1.0}};

void gfxInit();
void draw();

int main( int argc, char **argv ) 
{
	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(50, 100); // TODO do something
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Shiny Ducks");
	glutDisplayFunc(draw);
	gfxInit();
	glutMainLoop();

	return 0;
}

void gfxInit() 
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, WIDTH/HEIGHT, 1.0f, 200.0f);
}

void draw() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// set up das camera
	gluLookAt( 1.0f, 1.0f,  0.0f, 
			   0.0f, 0.0f, -15.0f, 
			   0.0f, 1.0f,  0.0f );

	// draw das scene
	glTranslatef(0.0, 0.0, -15.0);
	Cube().draw();

	glFlush();
	glutSwapBuffers();
}