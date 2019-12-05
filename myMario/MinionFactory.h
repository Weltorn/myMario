#pragma once
#include "Minion.h"
#include "Goomba.h"
#include "Koopa.h"
#include "PlayerBullet.h"
#include "RedMushroom.h"
#include "Star.h"
#include "Flower.h"


class MinionFactory
{
public:
	MinionFactory();
	~MinionFactory();
	//根据类型和位置（相对窗口）生成怪物
	static Minion* getMinion(MINION_TYPE type,int px,int py);
};

