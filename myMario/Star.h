#pragma once
#include "Minion.h"
#include "Player.h"
class Star :
	public Minion
{
public:
	Star(LPCTSTR imgPath, int frameWidth, int frameHeight);
	virtual ~Star();

	//更新怪物状态
	virtual void update();
	virtual bool CollideWith(IN T_Map* map);
	virtual bool CollideWith(T_Sprite* target, int distance = 0);
	//重力作用
	virtual void gravityEffect();
	void updatePosition();
	virtual void updatePositionY();			//竖直移动
};

