#pragma once
#include "T_Sprite.h"
enum MINION_TYPE
{
	MINION_STAR,			// ����
	MINION_REDMUSHROOM,		// ��Ģ��
	MINION_GREENMUSHROOM,	// ��Ģ��
	MINION_FLOWER,			// ��

	MINION_GOOMBA,				// �����У�Ģ��һ���Ĺ��
	MINION_KOOPA,				// ������
};

class Minion :
	public T_Sprite
{
private:
	MINION_TYPE minionType;		//��������
public:
	Minion(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~Minion();
	
	virtual bool CollideWith(IN T_Map* map);
	virtual bool CollideWith(T_Sprite* target, int distance = 0);
	MINION_TYPE getMinionType() { return minionType; }

	//���¹���״̬
	virtual void update();
};

