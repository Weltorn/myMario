#pragma once
#include "Minion.h"
class PlayerBullet :
	public Minion
{
private:
	int explodeFrmSequence[3] = { 3,4,5 };
public:
	PlayerBullet(LPCTSTR imgPath, int frameWidth, int frameHeight);
	virtual ~PlayerBullet();
	virtual string ClassName() { return "PlayerBullet"; }
	
	//更新怪物状态
	virtual void update();
	virtual void PlayerBullet::Draw(HDC hdc);
	virtual bool CollideWith(IN T_Map* map);
	virtual bool CollideWith(T_Sprite* target, int distance = 0);
	virtual void playAnimation();
	//重力作用
	virtual void gravityEffect();
	void updatePosition();
	virtual void updatePositionY();			//竖直移动
	void explode();	
};

