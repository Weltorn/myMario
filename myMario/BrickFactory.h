#pragma once
#include "Brick.h"
class BrickFactory
{
public:
	BrickFactory();
	~BrickFactory();

	//�������ͺ�λ�ã���Ե�ͼ�����ɹ���
	static Brick* getBrick(BRICK_TYPE type, int px, int py);
};

