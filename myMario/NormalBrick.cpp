#include "NormalBrick.h"



NormalBrick::NormalBrick(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:Brick(imgPath, frameWidth, frameHeight)
{
	bounceHeight = 0;
}


NormalBrick::~NormalBrick()
{
}

// 砖头蹦跶
bool NormalBrick::Bounce()
{
	lastY = Y;														// lastY用于绘制空白背景
	int changeHeight = GetHeight() / 16;							// 单次变化的高度

	if (bounceHeight >= GetHeight())								// 跳跃完毕
	{
		active = false;
		bounceHeight = 0;											// 置零 准备下次计数
	}
	else if (bounceHeight < GetHeight() / 2)						// 上移
	{
		Y -= changeHeight;
		bounceHeight += changeHeight;
	}
	else if(bounceHeight >= GetHeight()/2 && bounceHeight <GetHeight()) // 下移
	{
		Y += changeHeight;
		bounceHeight += changeHeight;
	}
	return true;
	
}

