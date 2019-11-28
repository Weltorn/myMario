#pragma once
#include "Minion.h"
#include "Player.h"
class T_Scene;

class Goomba :
	public Minion
{
private:
	bool crashed;	//是否被压扁状态
public:
	Goomba::Goomba(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~Goomba();

	virtual bool CollideWith(T_Sprite* target, int distance = 0);

	virtual void playAnimation();

	void dead(int deathType);	//0:马上死亡，无动画；1：压扁；2：击飞
	void crashedAnimate();
	void turnOverAnimate();
	void draw(HDC hdc);
};

