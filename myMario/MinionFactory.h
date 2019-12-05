#pragma once
#include "Minion.h"
#include "Goomba.h"
#include "Koopa.h"
#include "PlayerBullet.h"
#include "RedMushroom.h"
#include "Star.h"
#include "Flower.h"


class MinionFactory
{
public:
	MinionFactory();
	~MinionFactory();
	//�������ͺ�λ�ã���Դ��ڣ����ɹ���
	static Minion* getMinion(MINION_TYPE type,int px,int py);
};

