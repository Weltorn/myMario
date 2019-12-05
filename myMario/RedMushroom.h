#pragma once
#include "Minion.h"
#include "Player.h"

class RedMushroom :
	public Minion
{
public:
	RedMushroom(LPCTSTR imgPath, int frameWidth, int frameHeight);
	virtual ~RedMushroom();

	virtual string ClassName() { return "RedMushroom"; }
	virtual bool CollideWith(T_Sprite* target, int distance = 0);

	void die(int deathType =0)
	{
		SetDead(true);
		SetVisible(false);
		SetActive(false);
	}
};

