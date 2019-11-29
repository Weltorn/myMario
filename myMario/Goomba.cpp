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
		GAME_DIR DIR = getCollideDir(target);
		switch (DIR)
		{
		case DIR_LEFT:			
		case DIR_RIGHT:	
			if (target->GetLayerTypeID() == LAYER_TYPE::LAYER_PLY&&target->IsActive())
			{				
				(dynamic_cast<Player*>(target))->playerDeath(false);
			}		
			break;
		case DIR_UP:
			if (target->GetLayerTypeID() == LAYER_TYPE::LAYER_PLY&&target->IsActive())
			{
				(dynamic_cast<Player*>(target))->startJump();
				(dynamic_cast<Player*>(target))->stopBooting();
				//dead(1);	//被压扁
				dead(2);	//被击飞
			}
			break;
		case DIR_DOWN:
			
			break;
		}
	}
	
	return isCollide;
}
//怪物死亡deathType 	0:马上死亡，无动画；1：压扁；2：击飞
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

//怪物被压扁的动画
void Goomba::crashedAnimate()
{
	switch (currentStep)
	{
	case 0:	
		currentFrmIndex = 2;	//被压瘪的帧图
		++currentStep;
		break;
	case 1:			
		if (eventTimer + 1500 <= GetTickCount())	//显示被压扁的状态1.5秒
		{
			++currentStep;
		}
		break;
	default:
		dead(0);
		break;
	}
}
void Goomba::turnOverAnimate()
{
	switch (currentStep)
	{
	case 0:
		SetRotation(TRANS_VFLIP_NOROT);	//垂直翻转
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

//更新帧图
void Goomba::updateFrame()
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
	if (active)
	{
		//帧图选择
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