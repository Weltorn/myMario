#include "NormalBrick.h"



NormalBrick::NormalBrick(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:Brick(imgPath, frameWidth, frameHeight)
{
	bounceHeight = 0;
}


NormalBrick::~NormalBrick()
{
}

// שͷ���Q
bool NormalBrick::Bounce()
{
	lastY = Y;														// lastY���ڻ��ƿհױ���
	int changeHeight = GetHeight() / 16;							// ���α仯�ĸ߶�

	if (bounceHeight >= GetHeight())								// ��Ծ���
	{
		active = false;
		bounceHeight = 0;											// ���� ׼���´μ���
	}
	else if (bounceHeight < GetHeight() / 2)						// ����
	{
		Y -= changeHeight;
		bounceHeight += changeHeight;
	}
	else if(bounceHeight >= GetHeight()/2 && bounceHeight <GetHeight()) // ����
	{
		Y += changeHeight;
		bounceHeight += changeHeight;
	}
	return true;
	
}

