#include "Goomba.h"
#include "T_Scene.h"


Goomba::Goomba(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:Minion(imgPath, frameWidth, frameHeight)
{
}

Goomba::~Goomba()
{
}

bool Goomba::CollideWith(T_Sprite* target, int distance)
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
		GAME_DIR DIR = getCollideDir(target);
		switch (DIR)
		{
		case DIR_LEFT:			
		case DIR_RIGHT:	
			if (target->GetLayerTypeID() == LAYER_TYPE::LAYER_PLY&&target->IsActive())
			{
				Util::myprintf(L"minion and player left or right collide now -----------------------\n");
				(dynamic_cast<Player*>(target))->playerDeath(false);
			}		
			break;
		case DIR_UP:
			if (target->GetLayerTypeID() == LAYER_TYPE::LAYER_PLY&&target->IsActive())
			{
				(dynamic_cast<Player*>(target))->startJump();
				(dynamic_cast<Player*>(target))->stopBooting();
				//dead(1);	//±»Ñ¹±â
				dead(2);	//±»»÷·É
			}
			break;
		case DIR_DOWN:
			
			break;
		}
	}
	
	return isCollide;
}

void Goomba::dead(int deathType)
{
	switch (deathType)
	{
	case 0 :
		SetDead(true);
		SetVisible(false);
		SetActive(false);
		break;
	case 1:
		SetActive(false);
		startEvent(EVENTTYPE::NPC_DEATH_CRASH);
		break;
	case 2:
		SetActive(false);
		startEvent(EVENTTYPE::NPC_DEATH_TURNOVER);
		break;
	default:
		SetDead(true);
		SetActive(false);
	}
}
void Goomba::crashedAnimate()
{
	switch (currentStep)
	{
	case 0:	
		currentFrmIndex = 2;	//±»Ñ¹±ñµÄÖ¡Í¼
		++currentStep;
		break;
	case 1:			
		if (eventTimer + 1500 <= GetTickCount())	//ÏÔÊ¾±»Ñ¹±âµÄ×´Ì¬1.5Ãë
		{
			++currentStep;
		}
		break;
	default:
		Util::myprintf(L"goomba crash end\n --------------------------------------");
		dead(0);
		break;
	}
}
void Goomba::turnOverAnimate()
{
	switch (currentStep)
	{
	case 0:
		SetRotation(TRANS_VFLIP_NOROT);	//´¹Ö±·­×ª
		onPlantform = false;
		speedY = 6;
		speedX = 2;
		++currentStep;
		break;
	case 1:
		gravityEffect();
		updatePosition();
		if (Y - GetRatioSize().cy >= T_Scene::getBarrier()->GetHeight())
		{
			++currentStep;
		}
		break;
	default:
		inEvent = false;
		eventId = -1;
	}
}

void Goomba::playAnimation()
{
	switch (eventId)
	{
	case NPC_DEATH_CRASH:
		crashedAnimate();
		break;
	case NPC_DEATH_TURNOVER:
		turnOverAnimate();
		break;
	default:
		inEvent = false;
		eventId = -1;
		break;
	}
}

//¸üÐÂÖ¡Í¼
void Goomba::updateFrame()
{
	//³¯Ïò¿ØÖÆ
	if (dir == DIR_LEFT)
	{
		frameRotate = TRANS_HFLIP_NOROT;
	}
	else if (dir == DIR_RIGHT)
	{
		frameRotate = TRANS_NONE;
	}
	if (active)
	{
		//Ö¡Í¼Ñ¡Ôñ
		LoopFrame(12, true);
		currentFrmIndex = frameSequence[forward];
	}
}
void Goomba::Draw(HDC hdc)
{	
	lastX = X;
	lastY = Y;

	spImg.PaintFrame(
		spImg.GetBmpHandle(), hdc, (int)X, (int)Y, currentFrmIndex,
		frameCols, Width, Height, frameRatio, frameRotate, frameAlpha
	);
}