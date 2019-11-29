#pragma once
#include "T_Sprite.h"
#include "Brick.h"
class PropBrick :
	public Brick
{
public:
	PropBrick(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~PropBrick();
};

