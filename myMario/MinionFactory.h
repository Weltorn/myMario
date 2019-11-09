#pragma once
#include "Minion.h"
class MinionFactory
{
public:
	MinionFactory();
	~MinionFactory();
	//根据类型和位置（相对窗口）生成怪物
	Minion* getMinion(MINION_TYPE type,int px,int py);
};

