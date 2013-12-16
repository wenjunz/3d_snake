#include "Geometry.h"



Geometry::Geometry(void)
{
	for(int j = 0; j<6; j++)
	{
		for(int i = 0; i<SIZE*SIZE; i++)
		{
			block[i+j*SIZE*SIZE] = new Block(i+j*10000);
		}
	}
	string = false;
}

Geometry::~Geometry()
{
	for(int i = 0; i<SIZE*SIZE*6; i++)
	{
		delete block[i];
	}
}

void Geometry::DrawGeo(void)
{
	for(int j = 0; j<6; j++)
	{
		//int j=0;
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		if(j<4) glRotatef(j*90,0,1,0);
		if(j==4) glRotatef(90,1,0,0);
		if(j==5) glRotated(-90,1,0,0);
		for(int i = 0; i<SIZE*SIZE; i++)
		{
			float dy = floor(block[SIZE*SIZE*j+i]->index % 10000 % SIZE + 1 - SIZE/2.0); //index % SIZE + 1 is the row
			float dx = floor(block[SIZE*SIZE*j+i]->index % 10000 / SIZE + 1 - SIZE/2.0); //index / SIZE + 1 is the column
			//glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glTranslatef(0,0,SIZE/2.0);
			glTranslatef(dx,dy,0);
			block[SIZE*SIZE*j+i]->Draw();
			glPopMatrix();
		}
		glPopMatrix();
	}


}

void Geometry::SetSnake(int index, bool y)
{
	int i = index % 10000;
	int j = index / 10000;
	block[j*SIZE*SIZE+i]->snake = y;
}

void Geometry::SetBonus(int index, bool y)
{
	int i = index % 10000;
	int j = index / 10000;
	block[j*SIZE*SIZE+i]->bonus = y;
}

void Geometry::SetDead(int index, bool y)
{
	int i = index % 10000;
	int j = index / 10000;
	block[j*SIZE*SIZE+i]->dead_snake = y;
}

bool Geometry::GetDead(int index)
{
	int i = index % 10000;
	int j = index / 10000;
	return block[j*SIZE*SIZE+i]->dead_snake;
}

/*void Geometry::output(int x, int y, char *string)
{
  int len, i;

  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}*/