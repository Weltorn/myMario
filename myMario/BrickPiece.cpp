#include "BrickPiece.h"



BrickPiece::BrickPiece(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:Minion(imgPath, frameWidth, frameHeight)
{
}


BrickPiece::~BrickPiece()
{
}

//����֡ͼ
void BrickPiece::updateFrame()
{
	//�������
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
	//	//֡ͼѡ��
	//	LoopFrame(12, true);
	//	currentFrmIndex = frameSequence[forward];
	//}

	//֡ͼ���仯
	currentFrmIndex = 0;
}

void BrickPiece::update()
{
	if (!inEvent)
	{		
		updatePosition();	//�����������
		updateFrame();		//����֡ͼ				
	}
	else
	{
		playAnimation();
	}
}