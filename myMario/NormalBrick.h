#pragma once
#include "T_Sprite.h"
#include "Brick.h"

class NormalBrick :
	public Brick
{
protected:
	//int lastY;			//��ͨש���������ƶ�
	int bounceHeight;		// ���ĵĸ߶�
public:
	NormalBrick(LPCTSTR imgPath, int frameWidth, int frameHeight);
	~NormalBrick();
	void SetBounceHeight(int h) { bounceHeight = h; }
	int GetBounceHeight() { return bounceHeight; }
	bool Bounce();			// ��СMarioײ������Qһ��
//	int GetLastY() { return lastY; }
};

