#pragma once
#include "Minion.h"
#include "Goomba.h"

class MinionFactory
{
public:
	MinionFactory();
	~MinionFactory();
	//�������ͺ�λ�ã���Դ��ڣ����ɹ���
	static Minion* getMinion(MINION_TYPE type,int px,int py);
};

