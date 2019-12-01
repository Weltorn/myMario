#pragma once
#include "Minion.h"
class PlayerBullet :
	public Minion
{
public:
	PlayerBullet(LPCTSTR imgPath, int frameWidth, int frameHeight);
	virtual ~PlayerBullet();
	virtual string ClassName() { return "PlayerBullet"; }
	
	//¸üÐÂ¹ÖÎï×´Ì¬
	virtual void update();
	virtual void PlayerBullet::Draw(HDC hdc);
	virtual bool CollideWith(IN T_Map* map);
	virtual bool CollideWith(T_Sprite* target, int distance = 0);
	virtual void playAnimation();

	void explode();	
};

