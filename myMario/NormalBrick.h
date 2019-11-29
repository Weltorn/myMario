#pragma once
#include "T_Sprite.h"
#include "Brick.h"

class NormalBrick :
	public Brick
{
protected:
//	int lastX;		
	int lastY;			//普通砖块会可上下移动
public:
	NormalBrick(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~NormalBrick();
	void setLastY(int ly) { lastY = ly; }
	int getLastY() { return lastY; }
};

