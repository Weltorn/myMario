#pragma once
#include "Minion.h"
class PlayerBullet :
	public Minion
{
private:
	int explodeFrmSequence[3] = { 3,4,5 };
public:
	PlayerBullet(LPCTSTR imgPath, int frameWidth, int frameHeight);
	virtual ~PlayerBullet();
	virtual string ClassName() { return "PlayerBullet"; }
	
	//���¹���״̬
	virtual void update();
	virtual void PlayerBullet::Draw(HDC hdc);
	virtual bool CollideWith(IN T_Map* map);
	virtual bool CollideWith(T_Sprite* target, int distance = 0);
	virtual void playAnimation();
	//��������
	virtual void gravityEffect();
	void updatePosition();
	virtual void updatePositionY();			//��ֱ�ƶ�
	void explode();	
};

