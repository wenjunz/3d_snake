#pragma once
#include <vector>
using namespace std;

#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>

#include "Block.h"

#define SIZE 15

class Geometry
{
public:
	Geometry(void);
	virtual ~Geometry(void);

	void DrawGeo(void);

	void SetSnake(int index, bool y);
	void SetBonus(int index, bool y);
	void SetDead(int index, bool y);
	bool GetDead(int index);

	bool string;

private:
	Block* block[SIZE*SIZE*6];
	
	//void output(int x, int y, char *string);
};