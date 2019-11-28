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
	~Goomba();

	virtual bool CollideWith(T_Sprite* target, int distance = 0);

	virtual void playAnimation();

	void dead(int deathType);	//0:�����������޶�����1��ѹ�⣻2������
	void crashedAnimate();
	void turnOverAnimate();
	void draw(HDC hdc);
};

