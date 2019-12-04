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
	virtual ~Goomba();
	virtual string ClassName() { return "Goomba"; }
	virtual bool CollideWith(T_Sprite* target, int distance = 0);
	virtual void updateFrame();
	virtual void playAnimation();

	void die(int deathType);	//怪物死亡deathType 	0:马上死亡，无动画；2：压扁；1：击飞
	void crashedAnimate();
	void turnOverAnimate();
};

