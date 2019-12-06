#include "RedMushroom.h"
#include "GameScene.h"



RedMushroom::RedMushroom(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:Minion(imgPath, frameWidth, frameHeight)
{
}


RedMushroom::~RedMushroom()
{
}

bool RedMushroom::CollideWith(T_Sprite* target, int distance)
{
	bool isCollide = false;
	//¼ÆËã²ÎÓëÅö×²¼ì²âµÄ½ÇÉ«¾ØÐÎÇøÓò
	RECT targetRect = *(target->GetCollideRect());
	RECT hitRec;
	hitRec.left = targetRect.left - distance;
	hitRec.top = targetRect.top - distance;
	hitRec.right = targetRect.right + distance;
	hitRec.bottom = targetRect.bottom + distance;

	RECT* thisRect = this->GetCollideRect();

	//ÅÐ¶ÏÊÇ·ñÅö×²
	if (thisRect->left <= hitRec.right &&hitRec.left <= thisRect->right &&
		thisRect->top <= hitRec.bottom &&hitRec.top <= thisRect->bottom)
	{
		isCollide = true;
		int x = GetX(), y = GetY();
		
		if (target->GetLayerTypeID() == LAYER_TYPE::LAYER_PLY&&target->IsActive())
		{
			if ((dynamic_cast<Player*>(target))->getPlayerStatus() == PLAYERSTATUS::PLAYER_NORMAL)
			{
				(dynamic_cast<Player*>(target))->setPlayerMode(PLAYER_NORMAL);
				(dynamic_cast<Player*>(target))->startEvent(EVENTTYPE::PLAYER_LEVELUP);
			}
			else
				(dynamic_cast<Player*>(target))->startEvent(EVENTTYPE::PLAYER_LEVELDOWN);
			die(0);
		}

	}

	return isCollide;
}
