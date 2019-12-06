#include "BrickPiece.h"



BrickPiece::BrickPiece(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:Minion(imgPath, frameWidth, frameHeight)
{
}


BrickPiece::~BrickPiece()
{
}

//更新帧图
void BrickPiece::updateFrame()
{
	//朝向控制
	if (dir == DIR_LEFT)
	{
		frameRotate = TRANS_HFLIP_NOROT;
	}
	else if (dir == DIR_RIGHT)
	{
		frameRotate = TRANS_NONE;
	}
	//if (active)
	//{
	//	//帧图选择
	//	LoopFrame(12, true);
	//	currentFrmIndex = frameSequence[forward];
	//}

	//帧图不变化
	currentFrmIndex = 0;
}

void BrickPiece::update()
{
	if (!inEvent)
	{		
		updatePosition();	//更新玩家坐标
		updateFrame();		//更新帧图				
	}
	else
	{
		playAnimation();
	}
}