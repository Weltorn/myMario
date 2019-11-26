#pragma once
#include "Minion.h"
class Goomba :
	public Minion
{
public:
	Goomba::Goomba(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~Goomba();

	//bool CollideWith(T_Sprite* target, int distance = 0);
	//virtual void startEvent(int eventId);
	//virtual void playAnimation();
};

