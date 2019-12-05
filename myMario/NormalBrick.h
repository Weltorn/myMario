#pragma once
#include "T_Sprite.h"
#include "Brick.h"

class NormalBrick :
	public Brick
{
protected:
	//int lastY;			//普通砖块会可上下移动
	int bounceHeight;		// 跳的的高度
public:
	NormalBrick(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~NormalBrick();
	void SetBounceHeight(int h) { bounceHeight = h; }
	int GetBounceHeight() { return bounceHeight; }
	bool Bounce();			// 被小Mario撞击后蹦跶一下
//	int GetLastY() { return lastY; }
};

