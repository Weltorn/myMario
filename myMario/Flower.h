#pragma once
#include "Minion.h"
#include "Player.h"

class Flower :
	public Minion
{
public:
	Flower(LPCTSTR imgPath, int frameWidth, int frameHeight);
	virtual ~Flower();

	//¸üÐÂ¹ÖÎï×´Ì¬
	virtual void update();

	virtual bool CollideWith(T_Sprite* target, int distance = 0);
};

