#pragma once
#include "T_Sprite.h"
class T_Scene;
class Player :
	public T_Sprite
{
public:
	Player(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0);
	~Player();

	// ����ɫ��ײ, distance�����ײ�ľ���
	virtual bool CollideWith(T_Sprite* target, int distance = 0)override { return false; }
	// ����ͼ��ײ
	virtual bool CollideWith(IN T_Map* map)override { return false; }
	//���ݸ������״̬��Ϣ������λ�á�֡ͼ������ͼ��ײ
	virtual void update();
};

