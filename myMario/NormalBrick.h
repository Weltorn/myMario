#pragma once
#include "T_Sprite.h"
#include "Brick.h"

class NormalBrick :
	public Brick
{
public:
	NormalBrick(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~NormalBrick();
};

