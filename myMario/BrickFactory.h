#pragma once
#include "Brick.h"
class BrickFactory
{
public:
	BrickFactory();
	~BrickFactory();

	//根据类型和位置（相对地图）生成怪物
	static Brick* getBrick(BRICK_TYPE type, int px, int py);
};

