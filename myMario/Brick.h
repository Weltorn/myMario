#pragma once
#include "T_Sprite.h"

enum BRICK_TYPE
{
	NORMAL_BRICK,		// ��ͨש��
	PROP_BRICK,			// ����ש
	CRACK_BRICK,		// ש����Ƭ
};

class Brick :
	public T_Sprite
{
public:
	Brick(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~Brick();
};

