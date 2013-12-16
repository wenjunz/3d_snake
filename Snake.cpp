# include "Snake.h"

Snake::Snake(void)
{
	_Init();
}

Snake::~Snake(void)
{
	delete geometry;
}

void Snake::HitTest(void)
{
	int head = body.front();
	int face = head / 10000;
	int row = head % 10000 % SIZE + 1;
	int column = head % 10000 / SIZE + 1;
	
	int head_next = _HeadNext(face, row, column);	
	
	switch(_HitTest(head, head_next))
	{
	case 0: // Hit the wall
		break;
	case 1: // Normal
		Move(head_next);
		break;
	case 2: // Hit bonus
		Eat(head_next);
		break;
	case 3: // Hit itself
		_Dead();
		break;
	}
	_EyeLook(face, row, column);
}

int Snake::_HeadNext(int face, int row, int column)
{
	if(v_x > 0 && column == SIZE)
	{
		switch(face) // go out the face through right
		{
			case 0:
				{
					return 10000 + (row-1) + (1-1) * SIZE;
				}
			case 1:
				{
					return 20000 + (row-1) + (1-1) * SIZE;
				}
			case 2:
				{
					return 30000 + (row-1) + (1-1) * SIZE;
				}
			case 3:
				{
					return 00000 + (row-1) + (1-1) * SIZE;
				}
			case 4:
				{
					v_y = v_x;	v_x = 0;
					return 10000 + (1 - 1) + (SIZE-row) * SIZE;
				}
			case 5:
				{
					v_y = -v_x;	v_x = 0;
					return 10000 + (SIZE-1) + (row-1) * SIZE;
				}
		}
	}

	if(v_x < 0 && column == 1)
	{
		switch(face) // go out the face through left
		{
			case 0:
				{
					return 30000 + (row-1) + (SIZE-1) * SIZE;
				}
			case 1:
				{
					return 00000 + (row-1) + (SIZE-1) * SIZE;
				}
			case 2:
				{
					return 10000 + (row-1) + (SIZE-1) * SIZE;
				}
			case 3:
				{
					return 20000 + (row-1) + (SIZE-1) * SIZE;
				}
			case 4:
				{
					v_y = -v_x;	v_x = 0;
					return 30000 + (1 - 1) + (row-1) * SIZE;
				}
			case 5:
				{
					v_y = v_x;	v_x = 0;
					return 30000 + (SIZE-1) + (SIZE-row) * SIZE;
				}
		}
	}
	
	if(v_y > 0 && row == SIZE)
	{
		switch(face) // go out the face through up
		{
			case 0:
				{
					return 50000 + (1-1) + (column-1) * SIZE;
				}
			case 1:
				{
					v_x = -v_y;	v_y = 0;
					return 50000 + (column-1) + (SIZE-1) * SIZE;
				}
			case 2:
				{
					v_y = -v_y;
					return 50000 + (SIZE-1) + (SIZE-column) * SIZE;
				}
			case 3:
				{
					v_x = v_y;	v_y = 0;
					return 50000 + (SIZE-column) + (1-1) * SIZE;
				}
			case 4:
				{
					return 00000 + (1 - 1) + (column-1) * SIZE;
				}
			case 5:
				{
					v_y = -v_y;
					return 20000 + (SIZE-1) + (SIZE-column) * SIZE;
				}
		}
	}

	if(v_y < 0 && row == 1)
	{
		switch(face) // go out the face through bottom
		{
			case 0:
				{
					return 40000 + (SIZE-1) + (column-1)*SIZE;
					break;
				}
			case 1:
				{
					v_x = v_y;	v_y = 0;
					return 40000 + (SIZE-column) + (SIZE-1)*SIZE;
				}
			case 2:
				{
					v_y = -v_y;
					return 40000 + (1-1) + (SIZE-column) * SIZE;
				}
			case 3:
				{
					v_x = -v_y;	v_y = 0;
					return 40000 + (column-1) + (1-1) * SIZE;
				}
			case 4:
				{
					v_y = -v_y;
					return 20000 + (1-1) + (SIZE-column) * SIZE;
				}
			case 5:
				{
					return 00000 + (SIZE-1) + (column-1) * SIZE;
				}
		}
	}
	return face*10000 + row+v_y - 1 + (column+v_x - 1) * SIZE;// inside the inner squre
	
}
int Snake::_HitTest(int head, int head_next)
{
	std::vector<int>::iterator iter_bonus;
	iter_bonus = std::find(bonus.begin(), bonus.end(), head_next);
	if(iter_bonus != bonus.end())
		return 2; // Hit bonus

	std::vector<int>::iterator iter_body;
	iter_body = std::find(body.begin(), body.end(), head_next);
	if(iter_body != body.end())
		return 3; // Hit itself

	return 1; // hit nothing
}


void Snake::Move(int head_next)
{
	vector<int>::iterator iter_body = body.begin();
	body.insert( iter_body, 1, head_next );
	geometry->SetSnake(body.front(),true);

	geometry->SetSnake(body.back(),false);
	body.pop_back();
}

void Snake::Eat(int head_next)
{
	std::vector<int>::iterator iter_body = body.begin();
	body.insert( iter_body, 1, head_next ); 
	geometry->SetSnake(body.front(),true);
	// the same as the first three lines of Move()
	
	std::vector<int>::iterator iter_bonus;
	iter_bonus = std::find(bonus.begin(), bonus.end(), head_next);

	for(unsigned int i=0; i<bonus.size(); i++)
	{
		if(head_next == bonus.at(i))
			geometry->SetBonus(bonus.at(i),false);
	}
	
	if(iter_bonus != bonus.end())
	{
		bonus.erase(iter_bonus);
	}

	int index;
	do
	index = _GenBonus();
	while(!index);
}

void Snake::Control(unsigned char key)
{
	switch (key)
	{
	case 'w':
		//if(v_y >= 0) // not down
		//{v_x = 0; v_y = 1; v_z = 0;}
		{
			if(velocity >= 15)
				velocity -= 5;
			break;
		}
	case 's':
		//if(v_y <= 0) // not up
		//{v_x = 0; v_y = -1; v_z = 0;}
		{
			if(velocity <= 35)
				velocity +=5;
			break;
		
		}		
	case 'a':
		//if(v_x <= 0) // not right
		//{v_x = -1; v_y = 0; v_z = 0;}
		{
			int temp = -v_y;
			v_y = v_x;
			v_x = temp;
		}
		break;
	case 'd':
		//if(v_x >= 0) // not left
		//{v_x = 1; v_y = 0; v_z = 0;}
		{
			int temp = v_y;
			v_y = -v_x;
			v_x = temp;
		}
		break;
	case 'n':
		_Init();
	default:
		break;
	}

//	HitTest();
}

void Snake::Timer(void)
{
	if(!dead && (counter_velocity > velocity - 1))
	{
		counter_velocity = 0;
		HitTest();
	}
	counter_velocity++;

	if(dead)
	{
		if(counter_dead > 15)
		{
			counter_dead = 0;
			_Dead();
		}
		counter_dead++;
	}
}

void Snake::DrawAll()
{
	if(geometry)
	geometry->DrawGeo();
}
int Snake::_GenBonus(void)
{
	int index = rand()%(SIZE*SIZE);
	int face = rand()%6;

	std::vector<int>::iterator iter_bonus;
	iter_bonus = std::find(bonus.begin(),bonus.end(),face*10000+index);
	if(iter_bonus != bonus.end())
		return 0;
	iter_bonus = std::find(body.begin(),body.end(),face*10000+index);
	if(iter_bonus != body.end())
		return 0;
	geometry->SetBonus(face*10000+index,true);
	bonus.push_back(face*10000+index);
	return 1;
}
void Snake::_EyeLook(int face, int row, int column)
{
	switch(face)
	{
		case 0:
			eye_x = (float)floor(column + v_x - SIZE/2.0);
			eye_y = (float)floor(row + v_y - SIZE/2.0);
			eye_z = SIZE/2.0 + 0.5f;
			look_y = (float)floor(row + 5*v_y - SIZE/2.0);
			look_x = (float)floor(column + 5*v_x - SIZE/2.0);
			look_z = SIZE/2.0 + 0.0f;
			up_x = 0;	up_y = 0;	up_z = 1;
			break;
		case 1:
			eye_x = SIZE/2.0 + 0.5f;
			eye_y = (float)floor(row + v_y - SIZE/2.0);
			eye_z = -(float)floor(column + v_x - SIZE/2.0);
			look_x = SIZE/2.0 + 0.0f;
			look_y = (float)floor(row + v_y*5 - SIZE/2.0);
			look_z = -(float)floor(column + v_x*5 - SIZE/2.0);
			up_x = 1;	up_y = 0;	up_z = 0;
			break;
		case 2:
			eye_x = -(float)floor(column + v_x - SIZE/2.0);
			eye_y = (float)floor(row + v_y - SIZE/2.0);
			eye_z = -SIZE/2.0 - 0.5f;;
			look_x = -(float)floor(column + 5*v_x - SIZE/2.0);
			look_y = (float)floor(row + 5*v_y - SIZE/2.0);
			look_z = -SIZE/2.0 - 0.0f;
			up_x = 0;	up_y = 0;	up_z = -1;
			break;
		case 3:
			eye_x = -SIZE/2.0 - 0.5f;
			eye_y = (float)floor(row + v_y - SIZE/2.0);
			eye_z = (float)floor(column + v_x - SIZE/2.0);
			look_x = -SIZE/2.0 - 0.0f;
			look_y = (float)floor(row + 5*v_y - SIZE/2.0);
			look_z = (float)floor(column + 5*v_x - SIZE/2.0);
			up_x = -1;	up_y = 0;	up_z = 0;
			break;
		case 4:
			eye_x = (float)floor(column + v_x - SIZE/2.0);
			eye_y = -SIZE/2.0 - 0.5f;
			eye_z = (float)floor(row + v_y - SIZE/2.0);
			look_x = (float)floor(column + 5*v_x - SIZE/2.0);
			look_y = -SIZE/2.0 - 0.0f;
			look_z = (float)floor(row + 5*v_y - SIZE/2.0);
			up_x = 0;	up_y = -1;	up_z = 0;
			break;
		case 5:
			eye_x = (float)floor(column + v_x - SIZE/2.0);
			eye_y = SIZE/2.0 + 0.5f;
			eye_z = -(float)floor(row + v_y - SIZE/2.0);
			look_x = (float)floor(column + 5*v_x - SIZE/2.0);
			look_y = SIZE/2.0 + 0.0f;
			look_z = -(float)floor(row + 5*v_y - SIZE/2.0);
			up_x = 0;	up_y = 1;	up_z = 0;
			break;
	}
}
void Snake::_Dead(void)
{
	dead = true;
	geometry->SetDead(body.back(),true);
	int i;
	for(i = body.size()-1; i>=0; --i)
	{
		if(!geometry->GetDead(body.at(i)))
		{
			geometry->SetDead(body.at(i),true);
			break;
		}
	}
//	if(i == -1)
//		_Init();
}
void Snake::_Init()
{
	if(geometry != NULL)
		delete geometry;
	geometry = new Geometry;

	body.clear();
	bonus.clear();

	body.push_back(7);
	body.push_back(6);
	body.push_back(5);
	body.push_back(4);
	//body.push_back(3);
	//body.push_back(2);
	//body.push_back(153);
	//body.push_back(50);


	//geometry->SetSnake(153,true);
	//geometry->SetSnake(50,true);
	//geometry->SetSnake(2,true);
	//geometry->SetSnake(3,true);
	geometry->SetSnake(4,true);
	geometry->SetSnake(5,true);
	geometry->SetSnake(6,true);
	geometry->SetSnake(7,true);

	v_x = 0;
	v_y = 1;
	v_z = 0;

	counter_velocity = 0;
	velocity = 35;

	//bonus.push_back(10);
	//bonus.push_back(50050);
	//geometry->SetBonus(10,true);
	//geometry->SetBonus(50050,true);
	_GenBonus();
	_GenBonus();

	srand( (unsigned int)time(NULL) );
	dead = false;
}