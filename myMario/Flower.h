#pragma once
#include "Minion.h"
#include "Player.h"

class Flower :
	public Minion
{
public:
	Flower(LPCTSTR imgPath, int frameWidth, int frameHeight);
	virtual ~Flower();

	//���¹���״̬
	virtual void update();

	virtual bool CollideWith(T_Sprite* target, int distance = 0);
};

