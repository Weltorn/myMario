#pragma once
#include "Minion.h"
class PlayerBullet :
	public Minion
{
public:
	PlayerBullet(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~PlayerBullet();

	//¸üÐÂ¹ÖÎï×´Ì¬
	virtual void update();
	virtual bool CollideWith(IN T_Map* map);
	virtual bool CollideWith(T_Sprite* target, int distance = 0);
	virtual void playAnimation();

	void explode();	
};

