#pragma once
#include "Minion.h"
class MinionFactory
{
public:
	MinionFactory();
	~MinionFactory();
	//�������ͺ�λ�ã���Դ��ڣ����ɹ���
	Minion* getMinion(MINION_TYPE type,int px,int py);
};

