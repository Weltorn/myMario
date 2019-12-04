#include "Koopa.h"
#include "GameScene.h"


Koopa::Koopa(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:Minion(imgPath, frameWidth, frameHeight)
{
}


Koopa::~Koopa()
{
}


bool Koopa::CollideWith(T_Sprite* target, int distance)
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
			if (inshell)
			{
				if (speedX == 0)
				{
					if (target->GetLayerTypeID() == LAYER_TYPE::LAYER_PLY&&target->IsActive())
					{
						SetDir(target->GetDir());
						speedX = 4;
					}
				}
				else
				{
					if (target->GetLayerTypeID() == LAYER_TYPE::LAYER_PLY&&target->IsActive())//ɱ�����
					{
						//(dynamic_cast<Player*>(target))->playerDeath(false);
						if ((dynamic_cast<Player*>(target))->getPlayerStatus() == PLAYERSTATUS::PLAYER_NORMAL)
							(dynamic_cast<Player*>(target))->startEvent(EVENTTYPE::PLAYER_LEVELUP);
						else
							(dynamic_cast<Player*>(target))->startEvent(EVENTTYPE::PLAYER_LEVELDOWN);
					}
					//if (target->GetLayerTypeID() == LAYER_TYPE::LAYER_NPC&&target->IsActive())//����NPC
					//{
					//	(dynamic_cast<Minion*>(target))->die(1);
					//}					
				}
			}
			else
			{
				if (target->GetLayerTypeID() == LAYER_TYPE::LAYER_PLY&&target->IsActive())
				{
					if ((dynamic_cast<Player*>(target))->getPlayerStatus() == PLAYERSTATUS::PLAYER_NORMAL)
						(dynamic_cast<Player*>(target))->startEvent(EVENTTYPE::PLAYER_LEVELUP);
					else
						(dynamic_cast<Player*>(target))->startEvent(EVENTTYPE::PLAYER_LEVELDOWN);
				}			
			}
			break;
		case DIR_UP:
			
				if (inshell)
				{
					if (speedX == 0)
					{
						if (target->GetLayerTypeID() == LAYER_TYPE::LAYER_PLY&&target->IsActive())
						{
							(dynamic_cast<Player*>(target))->startJump();
							(dynamic_cast<Player*>(target))->stopBooting();
							SetDir(target->GetDir());
							speedX = 4;
						}
					}
					else
					{
						if (target->GetLayerTypeID() == LAYER_TYPE::LAYER_PLY&&target->IsActive())//ɱ�����
						{
							//(dynamic_cast<Player*>(target))->playerDeath(false);
							if ((dynamic_cast<Player*>(target))->getPlayerStatus() == PLAYERSTATUS::PLAYER_NORMAL)
								(dynamic_cast<Player*>(target))->startEvent(EVENTTYPE::PLAYER_LEVELUP);
							else
								(dynamic_cast<Player*>(target))->startEvent(EVENTTYPE::PLAYER_LEVELDOWN);
						}
					}
				}
				else
				{
					if (target->GetLayerTypeID() == LAYER_TYPE::LAYER_PLY&&target->IsActive())
					{
						(dynamic_cast<Player*>(target))->startJump();
						(dynamic_cast<Player*>(target))->stopBooting();
						hideInshell();
					}					
				}				
			break;
		case DIR_DOWN:

			break;
		}
	}

	return isCollide;
}
//��������deathType 	0:�����������޶�����1��ѹ�⣻2������
void Koopa::die(int deathType)
{
	switch (deathType)
	{
	case 0:
		SetDead(true);
		SetVisible(false);
		SetActive(false);
		break;
	case 1:	
		SetActive(false);
		startEvent(EVENTTYPE::NPC_DEATH_TURNOVER);
		break;
	default:
		SetDead(true);
		SetActive(false);
	}
}


void Koopa::turnOverAnimate()
{
	switch (currentStep)
	{
	case 0:
		currentFrmIndex = 2;	//��������
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

void Koopa::playAnimation()
{
	switch (eventId)
	{
	case NPC_DEATH_TURNOVER:
		turnOverAnimate();
		break;
	default:
		inEvent = false;
		eventId = -1;
		break;
	}
}
//��������
void Koopa::hideInshell()
{
	inshell = true;
	speedX = 0;
}
//�ӿ��г���
void Koopa::getOutOfshell()
{
	inshell = false;
	speedX = 1;
}
//����֡ͼ
void Koopa::updateFrame()
{
	//�������
	if (dir == DIR_LEFT)
	{
		frameRotate = TRANS_NONE;
	}
	else if (dir == DIR_RIGHT)
	{
		frameRotate = TRANS_HFLIP_NOROT;
	}
	if (active)
	{
		//֡ͼѡ��		
		if (inshell)
		{
			currentFrmIndex = 2;
		}
		else
		{
			LoopFrame(12, true);
			currentFrmIndex = frameSequence[forward];
		}
	}
}