#pragma once
#include "T_Sprite.h"
enum MINION_TYPE
{
	MINION_STAR,			// 星星
	MINION_REDMUSHROOM,		// 红蘑菇
	MINION_GREENMUSHROOM,	// 绿蘑菇
	MINION_FLOWER,			// 花

	MINION_GOOMBA,				// 板栗仔（蘑菇一样的怪物）
	MINION_KOOPA,				// 库帕龟
};

class Minion :
	public T_Sprite
{
private:
	MINION_TYPE minionType;		//怪物类型
public:
	Minion(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~Minion();
	
	virtual bool CollideWith(IN T_Map* map);
	virtual bool CollideWith(T_Sprite* target, int distance = 0);
	MINION_TYPE getMinionType() { return minionType; }

	//更新怪物状态
	virtual void update();
};

