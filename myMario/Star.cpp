#include "Star.h"
#include "GameScene.h"



Star::Star(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:Minion(imgPath, frameWidth, frameHeight)
{
}


Star::~Star()
{
}

void Star::gravityEffect()
{
	float currentGravity = gravity;		//������Ծ	

	if ((GetTickCount() - timer)* currentGravity / 270 >6 - abs(speedY))
	{
		speedY -= 1;
		if (speedY >= 12)
		{
			speedY = 12;
		}
		timer = GetTickCount();
	}
}
//��ֱ�ƶ�
void Star::updatePositionY()
{
	gravityEffect();		//��������	
	Y = Y - speedY;
}
void Star::updatePosition()
{
	updatePositionX();
	updatePositionY();
}
void Star::update()
{	
	updatePosition();					//��������
	CollideWith(T_Scene::getBarrier());		//���ϰ�����ײ���
	CollideWith(T_Scene::getNormalBrick());	//��ש����ײ���
	CollideWith(T_Scene::getPropBrick());	//��ש����ײ���
	updateFrame();						//����֡ͼ			
}
bool Star::CollideWith(IN T_Map* map)
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

	// ���ݽ�ɫ�����ϡ��¡����ҵľ��������ж��ĸ���������Ϊ�ϰ�

	if (speedY <= 0)
	{
		for (int row = startRow; row <= endRow; ++row)
		{
			if (dir == DIR_RIGHT)
			{
				for (int col = startCol; col <= endCol; ++col)
				{
					// �����ǰ�������ڵ�ͼ���ڵ�ͼ������Ϊ��0���ͱ�ʾ���ϰ�
					if (map->getTile(col, row) != 0)
					{
						isCollide = true;
						mapBlockPT.x = col;	// ��¼��ǰ�ϰ�ͼ�����
						mapBlockPT.y = row;	// ��¼��ǰ�ϰ�ͼ�����
											//��ײ�ĵ�ͼ��
						RECT blockRect = { col*map->getTileWidth() + (map->GetX()) ,row*map->getTileHeight() + (map->GetY()),
							(col + 1)*map->getTileWidth() + (map->GetX()),(row + 1)*map->getTileHeight() + (map->GetY()) };

						int x = GetX(), y = GetY();
						GAME_DIR DIR = getCollideDir(blockRect, false);
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
							SetPosition(x, y);
							speedY = -abs(speedY);
							break;
						case DIR_DOWN:
							x = GetX();
							y = map->GetY() + (row)*map->getTileHeight() - (GetRatioSize().cy + colideHeight) / 2;  //�����ϰ��ϲ�
							SetPosition(x, y);
							speedY = 8;
							break;
						}
					}
				}
			}
			else if (dir == DIR_LEFT)
			{
				for (int col = endCol; col >= startCol; --col)
				{
					// �����ǰ�������ڵ�ͼ���ڵ�ͼ������Ϊ��0���ͱ�ʾ���ϰ�
					if (map->getTile(col, row) != 0)
					{
						isCollide = true;
						mapBlockPT.x = col;	// ��¼��ǰ�ϰ�ͼ�����
						mapBlockPT.y = row;	// ��¼��ǰ�ϰ�ͼ�����
											//��ײ�ĵ�ͼ��
						RECT blockRect = { col*map->getTileWidth() + (map->GetX()) ,row*map->getTileHeight() + (map->GetY()),
							(col + 1)*map->getTileWidth() + (map->GetX()),(row + 1)*map->getTileHeight() + (map->GetY()) };

						int x = GetX(), y = GetY();
						GAME_DIR DIR = getCollideDir(blockRect, false);
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
							SetPosition(x, y);
							speedY = -abs(speedY);
							break;
						case DIR_DOWN:
							x = GetX();
							y = map->GetY() + (row)*map->getTileHeight() - (GetRatioSize().cy + colideHeight) / 2;  //�����ϰ��ϲ�
							SetPosition(x, y);
							speedY = 8;
							break;
						}
					}
				}
			}
		}
	}
	else if (speedY > 0)
	{
		for (int row = endRow; row >= startRow; --row)
		{
			if (dir == DIR_RIGHT)
			{
				for (int col = startCol; col <= endCol; ++col)
				{
					// �����ǰ�������ڵ�ͼ���ڵ�ͼ������Ϊ��0���ͱ�ʾ���ϰ�
					if (map->getTile(col, row) != 0)
					{
						isCollide = true;
						mapBlockPT.x = col;	// ��¼��ǰ�ϰ�ͼ�����
						mapBlockPT.y = row;	// ��¼��ǰ�ϰ�ͼ�����
											//��ײ�ĵ�ͼ��
						RECT blockRect = { col*map->getTileWidth() + (map->GetX()) ,row*map->getTileHeight() + (map->GetY()),
							(col + 1)*map->getTileWidth() + (map->GetX()),(row + 1)*map->getTileHeight() + (map->GetY()) };

						int x = GetX(), y = GetY();
						GAME_DIR DIR = getCollideDir(blockRect, false);
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
							SetPosition(x, y);
							speedY = -abs(speedY);
							break;
						case DIR_DOWN:
							x = GetX();
							y = map->GetY() + (row)*map->getTileHeight() - (GetRatioSize().cy + colideHeight) / 2;  //�����ϰ��ϲ�
							SetPosition(x, y);
							speedY = 8;
							break;
						}
					}
				}
			}
			else if (dir == DIR_LEFT)
			{
				for (int col = endCol; col >= startCol; --col)
				{
					// �����ǰ�������ڵ�ͼ���ڵ�ͼ������Ϊ��0���ͱ�ʾ���ϰ�
					if (map->getTile(col, row) != 0)
					{
						isCollide = true;
						mapBlockPT.x = col;	// ��¼��ǰ�ϰ�ͼ�����
						mapBlockPT.y = row;	// ��¼��ǰ�ϰ�ͼ�����
											//��ײ�ĵ�ͼ��
						RECT blockRect = { col*map->getTileWidth() + (map->GetX()) ,row*map->getTileHeight() + (map->GetY()),
							(col + 1)*map->getTileWidth() + (map->GetX()),(row + 1)*map->getTileHeight() + (map->GetY()) };

						int x = GetX(), y = GetY();
						GAME_DIR DIR = getCollideDir(blockRect, false);
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
							SetPosition(x, y);
							speedY = -abs(speedY);
							break;
						case DIR_DOWN:
							x = GetX();
							y = map->GetY() + (row)*map->getTileHeight() - (GetRatioSize().cy + colideHeight) / 2;  //�����ϰ��ϲ�
							SetPosition(x, y);
							speedY = 8;
							break;
						}
					}
				}
			}
		}
	}
	return isCollide;
}
bool Star::CollideWith(T_Sprite* target, int distance)
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
		int type = (target)->GetLayerTypeID();
		if (type == LAYER_TYPE::LAYER_PLY)
		{					
			(dynamic_cast<Player*>(target))->setColorful();
			die();
		}
	}
	return isCollide;
}