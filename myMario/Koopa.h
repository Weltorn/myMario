#pragma once
#include "Minion.h"
#include "Player.h"
class Koopa :
	public Minion
{
private:
	bool inshell;	//缩在壳里
public:
	Koopa(LPCTSTR imgPath, int frameWidth, int frameHeight);
	virtual ~Koopa();
	virtual string ClassName() { return "Koopa"; }
	virtual bool CollideWith(T_Sprite* target, int distance = 0);
	virtual void updateFrame();
	virtual void playAnimation();

	void die(int deathType);	//0:马上死亡，无动画；1：击飞
	void hideInshell();			//缩进壳里
	void getOutOfshell();		//从壳中出来
	void turnOverAnimate();
};

