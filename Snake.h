#pragma once
#include <vector>
#include <algorithm>
using namespace std;

#include "Geometry.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


class Snake
{
public:
	Snake(void);
	virtual ~Snake(void);

	void HitTest(void);
	void Move(int head_next);
	void Eat(int head_next);
	void Control(unsigned char key);
	void Timer(void);

	void DrawAll();

	float eye_x, eye_y, eye_z;
	float look_x, look_y, look_z;
	float up_x, up_y, up_z;

private:
	vector<int> body;
	vector<int> bonus;
	Geometry* geometry;
	int v_x, v_y, v_z;
	unsigned int counter_velocity;
	unsigned int counter_dead;
	unsigned int velocity;
	bool dead;

	int _HeadNext(int face, int row, int column);
	int _HitTest(int head, int head_next);
	int _GenBonus();
	void _EyeLook(int face, int row, int column);
	void _Dead();
	void _Init();
};