#pragma once
#include "Minion.h"
#include "Player.h"
class Koopa :
	public Minion
{
private:
	bool inshell;	//���ڿ���
public:
	Koopa(LPCTSTR imgPath, int frameWidth, int frameHeight);
	virtual ~Koopa();
	virtual string ClassName() { return "Koopa"; }
	virtual bool CollideWith(T_Sprite* target, int distance = 0);
	virtual void updateFrame();
	virtual void playAnimation();

	void die(int deathType);	//0:�����������޶�����1������
	void hideInshell();			//��������
	void getOutOfshell();		//�ӿ��г���
	void turnOverAnimate();
};

