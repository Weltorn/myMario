#include "Flower.h"
#include "GameScene.h"



Flower::Flower(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:Minion(imgPath, frameWidth, frameHeight)
{
}


Flower::~Flower()
{
}

void Flower::update()
{			
	updateFrame();						//更新帧图			
}

bool Flower::CollideWith(T_Sprite* target, int distance)
{
	bool isCollide = false;
	//计算参与碰撞检测的角色矩形区域
	RECT targetRect = *(target->GetCollideRect());
	RECT hitRec;
	hitRec.left = targetRect.left - distance;
	hitRec.top = targetRect.top - distance;
	hitRec.right = targetRect.right + distance;
	hitRec.bottom = targetRect.bottom + distance;

	RECT* thisRect = this->GetCollideRect();
	//判断是否碰撞
	if (thisRect->left <= hitRec.right &&hitRec.left <= thisRect->right &&
		thisRect->top <= hitRec.bottom &&hitRec.top <= thisRect->bottom)
	{
		isCollide = true;
		int x = GetX(), y = GetY();
		int type = (target)->GetLayerTypeID();
		if (type == LAYER_TYPE::LAYER_PLY)
		{
			if ((dynamic_cast<Player*>(target))->getPlayerStatus() != PLAYERSTATUS::PLAYER_BIGRED)
				(dynamic_cast<Player*>(target))->startEvent(EVENTTYPE::PLAYER_LEVELUP);
			die(0);
		}
	}
	return isCollide;
}