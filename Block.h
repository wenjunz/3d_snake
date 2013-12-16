#pragma once

class Block
{
public:
	Block(int i);
	virtual ~Block(void);

	virtual void Draw(void);

	int index;
	bool snake;
	bool bonus;
	bool dead_snake;

private:
};