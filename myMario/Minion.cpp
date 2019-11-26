#include "Minion.h"
#include "T_Scene.h"
#include "GameMap.h"


Minion::Minion(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{
	gravity = 4;
}
Minion::~Minion()
{
}

// �жϹ�����Ŀ����ε���ײ��������ڹ���ķ���
GAME_DIR Minion::getCollideDir(RECT target)
{
	RECT oldRect = *this->GetCollideRect();
	RECT currentRect = *this->GetCollideRect();
	oldRect.left = this->GetCollideRect()->left - (X - lastX);
	oldRect.right = this->GetCollideRect()->right - (X - lastX);
	oldRect.top = this->GetCollideRect()->top - (Y - lastY);
	oldRect.bottom = this->GetCollideRect()->bottom - (Y - lastY);

	if (oldRect.left >= target.right && this->GetCollideRect()->left <= target.right)
	{
		return DIR_LEFT;
	}
	if (oldRect.right <= target.left && this->GetCollideRect()->right >= target.left)
	{
		return DIR_RIGHT;
	}
	if (oldRect.top >= target.bottom && this->GetCollideRect()->top <= target.bottom)
	{
		return DIR_UP;
	}
	if (oldRect.bottom <= target.top && this->GetCollideRect()->bottom >= target.top)
	{
		return DIR_DOWN;
	}
	return DIR_NONE;
}

bool Minion::CollideWith(IN T_Map* map)
{
	bool isCollide = false;
	// �������ΪͼƬ�򲻼���ͼ��ײ
	if (map->getMapRows() == 0 && map->getMapCols() == 0)
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// �����ͼ���ɼ����ɫ���ɼ�������ͼ��ײ
	if ((!(map->IsVisible())) || (!(this->IsVisible())))
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// ���㵱ǰ��ͼͼ��ľ��η�Χ
	int mapLeft = map->GetX();
	int mapTop = map->GetY();
	int mapRight = mapLeft + map->GetWidth();
	int mapBottom = mapTop + map->GetHeight();

	//��ͼ���ұ߽���ײ
	if (this->X <= mapLeft)
	{
		this->X = mapLeft;
	}
	if (this->X + this->GetRatioSize().cx >= mapRight)
	{
		this->X = mapRight - this->GetRatioSize().cx - 1;
	}
	// ��õ�ͼͼ����ʹ�õ�ͼ��Ŀ��
	int tW = map->getTileWidth();
	int tH = map->getTileHeight();

	// ���㵱ǰ��ɫ�ľ��η�Χ����ײ����
	int spLeft = this->GetCollideRect()->left;
	int spTop = this->GetCollideRect()->top;
	int spRight = this->GetCollideRect()->right;
	int spBottom = this->GetCollideRect()->bottom;

	// ��õ�ǰ��ͼ��ͼ���������
	int tNumCols = map->getMapCols();
	// ��õ�ǰ��ͼ��ͼ���������
	int tNumRows = map->getMapRows();

	// ���㵱ǰ��ɫ��ʵ�ʵĿ��
	int spW = spRight - spLeft;
	int spH = spBottom - spTop;

	RECT lprcDst;
	// �������ϼ����ͼ��ľ��η�Χ�ͽ�ɫ�ľ��η�Χ�����������ζ���
	RECT mapRect = { mapLeft, mapTop, mapRight, mapBottom };
	RECT spRect = { spLeft, spTop, spRight, spBottom };
	// ����������ζ���û�з����κ���ײ�����˳�����
	if ((IntersectRect(&lprcDst, &mapRect, &spRect)) == FALSE)
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// ����������ζ���������ײ�����ȼ����ɫ�����ϡ��¡����ҵľ�������
	int startRow = (spTop <= mapTop) ? 0 : (spTop - mapTop) / tH;
	int endRow = (spBottom < mapBottom) ? (spBottom - 1 - mapTop) / tH : tNumRows - 1;	//-1����Ե�ж�
	int startCol = (spLeft <= mapLeft) ? 0 : (spLeft - mapLeft) / tW;
	int endCol = (spRight < mapRight) ? (spRight - 1 - mapLeft) / tW : tNumCols - 1;

	COLLIDBLOCKS collideBlocks;
	// ���ݽ�ɫ�����ϡ��¡����ҵľ��������ж��ĸ���������Ϊ�ϰ�
	for (int row = startRow; row <= endRow; ++row)
	{
		for (int col = startCol; col <= endCol; ++col)
		{
			// �����ǰ�������ڵ�ͼ���ڵ�ͼ������Ϊ��0���ͱ�ʾ���ϰ�
			if (map->getTile(col, row) != 0)
			{
				isCollide = true;
				mapBlockPT.x = col;	// ��¼��ǰ�ϰ�ͼ�����
				mapBlockPT.y = row;	// ��¼��ǰ�ϰ�ͼ�����

				COLLIDBLOCK block;

				//��ײ�ĵ�ͼ��
				RECT blockRect = { col*map->getTileWidth() + (map->GetX()) ,row*map->getTileHeight() + (map->GetY()),
					(col + 1)*map->getTileWidth() + (map->GetX()),(row + 1)*map->getTileHeight() + (map->GetY()) };

				int x = GetX(), y = GetY();
				GAME_DIR DIR = getCollideDir(blockRect);
				switch (DIR)
				{
				case DIR_LEFT:
					x = map->GetX() + (col + 1)*map->getTileWidth();
					y = GetY();
					dir = DIR_RIGHT;					
					break;
				case DIR_RIGHT:
					x = map->GetX() + col*map->getTileWidth() - GetRatioSize().cx;
					y = GetY();
					dir = DIR_LEFT;					
					break;
				case DIR_UP:
					x = GetX();
					y = map->GetY() + (row + 1)*map->getTileHeight();		//�����ϰ��²�
					speedY = -abs(speedY);					
					break;
				case DIR_DOWN:
					x = GetX();
					y = map->GetY() + (row)*map->getTileHeight() - GetRatioSize().cy;  //�����ϰ��ϲ�
					onPlantform = true;
					speedY = 0;					
					break;
				}
				// ����ɫ��λ���ϰ���߽�
				SetPosition(x, y);
			}
		}
	}
	(dynamic_cast<GameMap*>(map))->setCollideBlocks(collideBlocks); //ˢ����ײ��ͼ��
	return isCollide;
}


bool Minion::CollideWith(T_Sprite* target, int distance)
{
	return false;
}

void Minion::updatePosition()
{
	updatePositionX();
	updatePositionY();
}
//��ֱ�ƶ�
void Minion::updatePositionY()
{
	gravityEffect();		//��������
	lastY = Y;
	Y = Y - speedY;
}
//ˮƽ�ƶ�						
void Minion::updatePositionX()
{
	int ispeedX;
	//���ݷ��������ٶȷ���
	if (dir == DIR_LEFT)
		ispeedX = -abs(speedX);
	else if (dir == DIR_RIGHT)
		ispeedX = abs(speedX);

	lastX = X;
	X += ispeedX;
}
//����֡ͼ
void Minion::updateFrame()
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

	//֡ͼѡ��
	LoopFrame();
	currentFrmIndex = frameSequence[forward];


}
void Minion::update()
{
	if (!inEvent)
	{
		checkOnplantForm(T_Scene::getBarrier());
		if (onPlantform)		//��ƽ̨�ϣ���ֱ�ٶ�Ϊ��
		{
			speedY = 0;
		}
		updatePosition();					//��������
		CollideWith(T_Scene::getBarrier());	//�ϰ�����ײ���
		CollideWith(T_Scene::getPlayer());	//��Ҳ���ײ���
		updateFrame();						//����֡ͼ
	}
	else
	{
		playAnimation();
	}
}

//����Ƿ���ƽ̨��
bool Minion::checkOnplantForm(T_Map* map)
{	
	if ((Y + GetRatioSize().cy - map->GetY()) % map->getTileHeight() < 2)
	{
		//�����·��ĵ�ͼ��
		int row = (Y + GetRatioSize().cy - map->GetY()) / map->getTileHeight();
		int startCol = (X - map->GetX() + 1) / map->getTileWidth();
		int endCol = (X + GetRatioSize().cx - map->GetX() - 1) / map->getTileWidth();

		//�Ƿ񳬳���ͼ��Χ
		if (row > map->getMapRows() - 1 || row < 0 || startCol<0 || endCol>map->getMapCols() - 1)
		{
			onPlantform = false;
			return onPlantform;
		}
		//�������·��ĵ�ͼ��
		for (int i = startCol; i <= endCol; i++)
		{
			if (map->getTile(i, row) != 0)
			{
				onPlantform = true;
				return onPlantform;
			}
		}
	}
	onPlantform = false;
	return onPlantform;
}

//��������
void  Minion::gravityEffect()
{
	float currentGravity = gravity;		//������Ծ	
	
	if (!onPlantform && (GetTickCount() - timer)* currentGravity / 270 >abs(speedY))
	{
		speedY -= 1;	
	}	
	if (speedY == 0)		//���¼�ʱ��
	{
		timer = GetTickCount();
	}
}


// ��ʼ�¼�
void Minion::startEvent(int eventId)
{
	this->eventId = eventId;
	inEvent = true;
	currentStep = 0;
	eventTimer = GetTickCount();

	speedX = 0;
	speedY = 0;
	playAnimation();
}

//�����¼�����
void Minion::playAnimation()
{
	switch (eventId)
	{
	case PLAYER_DEATH:
		
		break;
	case PLAYER_LEVELUP:
		
		break;
	case PLAYER_LEVELDOWN:
		break;
	case PLAYER_AFTERPOLE:
		break;
	default:
		inEvent = false;
		eventId = -1;
		break;
	}
}
