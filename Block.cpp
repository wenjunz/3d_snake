#include "Block.h"
#include <GL/glut.h>
#include <math.h>


Block::Block(int i)
{
	snake = false;
	bonus = false;
	dead_snake = false;
	index = i;
}

Block::~Block(void)
{
}

void Block::Draw(void)
{
	/*glColor3f(1,0,0);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(-0.5,-0.5,0);
	glVertex3f(-0.5,0.5,0);
	glVertex3f(0.5,0.5,0);
	glVertex3f(0.5,-0.5,0);
	glEnd();*/

	if(snake)
	{
		glPushMatrix();
		glTranslatef(0,0,0.5);
		if(dead_snake)
			glColor3f(0.2,0.2,0.2);
		else
			glColor3f(0.0,0.6,0.0);
		glutSolidCube(0.6);
		glPopMatrix();
	}

	if(bonus)
	{
		glPushMatrix();
		glTranslatef(0,0,0.5);
		glColor3f(0.6,0.0,0.0);
		glutSolidCube(0.6);
		glPopMatrix();
	}

	glColor3f(0,0,0.5);
	glBegin(GL_LINE_LOOP);
	glNormal3f(0,0,1);

	glVertex3f(-0.499,-0.499,0);
	glVertex3f(-0.499,0.499,0);
	glVertex3f(0.499,0.499,0);
	glVertex3f(0.499,-0.499,0);
	glEnd();

	glColor3f(0.5,0.3,0.0);
	glBegin(GL_LINE_LOOP);
	glNormal3f(0,0,1);
	glVertex3f(-0.4,-0.4,0);
	glVertex3f(-0.4,0.4,0);
	glVertex3f(0.4,0.4,0);
	glVertex3f(0.4,-0.4,0);
	glEnd();
}
