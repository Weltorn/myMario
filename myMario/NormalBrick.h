#pragma once
#include "T_Sprite.h"
#include "Brick.h"

class NormalBrick :
	public Brick
{
protected:
//	int lastX;		
	int lastY;			//��ͨש���������ƶ�
public:
	NormalBrick(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~NormalBrick();
	void setLastY(int ly) { lastY = ly; }
	int getLastY() { return lastY; }
};

