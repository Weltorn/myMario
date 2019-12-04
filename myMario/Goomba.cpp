#include "Goomba.h"
#include "GameScene.h"


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
	//���������ײ���Ľ�ɫ��������
	RECT targetRect = *(target->GetCollideRect());
	RECT hitRec;
	hitRec.left = targetRect.left - distance;
	hitRec.top = targetRect.top - distance;
	hitRec.right = targetRect.right + distance;
	hitRec.bottom = targetRect.bottom + distance;

	RECT* thisRect = this->GetCollideRect();

	//�ж��Ƿ���ײ
	if (thisRect->left <= hitRec.right &&hitRec.left <= thisRect->right &&
		thisRect->top <= hitRec.bottom &&hitRec.top <= thisRect->bottom)
	{
		isCollide = true;
		int x = GetX(), y = GetY();
		GAME_DIR DIR = getCollideDir(target,distance);
		switch (DIR)
		{
		case DIR_LEFT:			
		case DIR_RIGHT:	
			if (target->GetLayerTypeID() == LAYER_TYPE::LAYER_PLY&&target->IsActive())
			{		
				//(dynamic_cast<Player*>(target))->playerDeath(false);
				if((dynamic_cast<Player*>(target))->getPlayerStatus() == PLAYERSTATUS::PLAYER_NORMAL)				
					(dynamic_cast<Player*>(target))->startEvent(EVENTTYPE::PLAYER_LEVELUP);
				else
					(dynamic_cast<Player*>(target))->startEvent(EVENTTYPE::PLAYER_LEVELDOWN);
			}		
			break;
		case DIR_UP:
			if (target->GetLayerTypeID() == LAYER_TYPE::LAYER_PLY&&target->IsActive())
			{
				(dynamic_cast<Player*>(target))->startJump();
				(dynamic_cast<Player*>(target))->stopBooting();
				die(2);	//��ѹ��
			}
			break;
		case DIR_DOWN:
			
			break;
		}
	}
	
	return isCollide;
}
//��������deathType 	0:�����������޶�����2��ѹ�⣻1������
void Goomba::die(int deathType)
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
		startEvent(EVENTTYPE::NPC_DEATH_TURNOVER);
		break;
	case 2:
		SetActive(false);
		startEvent(EVENTTYPE::NPC_DEATH_CRASH);
		break;
	default:
		SetDead(true);
		SetActive(false);
	}
}

//���ﱻѹ��Ķ���
void Goomba::crashedAnimate()
{
	switch (currentStep)
	{
	case 0:	
		currentFrmIndex = 2;	//��ѹ���֡ͼ
		++currentStep;
		break;
	case 1:			
		if (eventTimer + 1500 <= GetTickCount())	//��ʾ��ѹ���״̬1.5��
		{
			++currentStep;
		}
		break;
	default:
		die(0);
		break;
	}
}
void Goomba::turnOverAnimate()
{
	switch (currentStep)
	{
	case 0:
		SetRotation(TRANS_VFLIP_NOROT);	//��ֱ��ת
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

//����֡ͼ
void Goomba::updateFrame()
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
	if (active)
	{
		//֡ͼѡ��
		LoopFrame(12, true);
		currentFrmIndex = frameSequence[forward];
	}
}