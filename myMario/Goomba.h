#pragma once
#include "Minion.h"
#include "Player.h"
class T_Scene;

class Goomba :
	public Minion
{
private:
	bool crashed;	//�Ƿ�ѹ��״̬
public:
	Goomba::Goomba(LPCTSTR imgPath, int frameWidth, int frameHeight);
	virtual ~Goomba();
	virtual string ClassName() { return "Goomba"; }
	virtual bool CollideWith(T_Sprite* target, int distance = 0);
	virtual void updateFrame();
	virtual void playAnimation();

	void die(int deathType);	//��������deathType 	0:�����������޶�����2��ѹ�⣻1������
	void crashedAnimate();
	void turnOverAnimate();
};

