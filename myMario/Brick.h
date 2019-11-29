#pragma once
#include "T_Sprite.h"

enum BRICK_TYPE
{
	NORMAL_BRICK,		// ÆÕÍ¨×©¿é
	PROP_BRICK,			// µÀ¾ß×©
	CRACK_BRICK,		// ×©¿éËéÆ¬
};

class Brick :
	public T_Sprite
{
public:
	Brick(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~Brick();
};

