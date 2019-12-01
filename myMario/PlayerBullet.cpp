#include "PlayerBullet.h"
#include "T_Scene.h"
#include "GameMap.h"


PlayerBullet::PlayerBullet(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:Minion(imgPath, frameWidth, frameHeight)
{
	frameFrequence = 6;
}


PlayerBullet::~PlayerBullet()
{
	delete frameSequence;
}
void PlayerBullet::update()
{
	if (!inEvent)
	{
		checkOnplantForm(T_Scene::getBarrier());
		if (onPlantform)		//在平台上，竖直速度为零
		{
			speedY = -speedY;
		}

		updatePosition();					//更新坐标
		CollideWith(T_Scene::getBarrier());	//障碍层碰撞检测		
		updateFrame();						//更新帧图		
	}
	else
	{
		playAnimation();
	}
}
bool PlayerBullet::CollideWith(IN T_Map* map)
{
	bool isCollide = false;
	// 如果背景为图片则不检测地图碰撞
	if (map->getMapRows() == 0 && map->getMapCols() == 0)
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// 如果地图不可见或角色不可见不检测地图碰撞
	if ((!(map->IsVisible())) || (!(this->IsVisible())))
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// 计算当前地图图层的矩形范围
	int mapLeft = map->GetX();
	int mapTop = map->GetY();
	int mapRight = mapLeft + map->GetWidth();
	int mapBottom = mapTop + map->GetHeight();

	//地图左右边界碰撞
	if (this->X <= mapLeft)
	{
		this->X = mapLeft;
	}
	if (this->X + this->GetRatioSize().cx >= mapRight)
	{
		this->X = mapRight - this->GetRatioSize().cx - 1;
	}
	// 获得地图图层中使用的图块的宽高
	int tW = map->getTileWidth();
	int tH = map->getTileHeight();

	// 计算当前角色的矩形范围（碰撞区域）
	int spLeft = this->GetCollideRect()->left;
	int spTop = this->GetCollideRect()->top;
	int spRight = this->GetCollideRect()->right;
	int spBottom = this->GetCollideRect()->bottom;

	// 获得当前地图中图块的总列数
	int tNumCols = map->getMapCols();
	// 获得当前地图中图块的总行数
	int tNumRows = map->getMapRows();

	// 计算当前角色的实际的宽高
	int spW = spRight - spLeft;
	int spH = spBottom - spTop;

	RECT lprcDst;
	// 根据以上计算的图层的矩形范围和角色的矩形范围构造两个矩形对象
	RECT mapRect = { mapLeft, mapTop, mapRight, mapBottom };
	RECT spRect = { spLeft, spTop, spRight, spBottom };
	// 如果两个矩形对象没有发生任何碰撞，则退出函数
	if ((IntersectRect(&lprcDst, &mapRect, &spRect)) == FALSE)
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	// 如果两个矩形对象发生了碰撞，首先计算角色矩形上、下、左、右的矩形区域
	int startRow = (spTop <= mapTop) ? 0 : (spTop - mapTop) / tH;
	int endRow = (spBottom < mapBottom) ? (spBottom - 1 - mapTop) / tH : tNumRows - 1;	//-1，边缘判断
	int startCol = (spLeft <= mapLeft) ? 0 : (spLeft - mapLeft) / tW;
	int endCol = (spRight < mapRight) ? (spRight - 1 - mapLeft) / tW : tNumCols - 1;

	// 根据角色矩形上、下、左、右的矩形区域判断哪个矩形区域为障碍
	for (int row = startRow; row <= endRow; ++row)
	{
		for (int col = startCol; col <= endCol; ++col)
		{
			// 如果当前矩形所在的图块在地图数据中为非0，就表示是障碍
			if (map->getTile(col, row) != 0)
			{
				isCollide = true;
				mapBlockPT.x = col;	// 记录当前障碍图块的列
				mapBlockPT.y = row;	// 记录当前障碍图块的行
				//碰撞的地图块
				RECT blockRect = { col*map->getTileWidth() + (map->GetX()) ,row*map->getTileHeight() + (map->GetY()),
					(col + 1)*map->getTileWidth() + (map->GetX()),(row + 1)*map->getTileHeight() + (map->GetY()) };

				int x = GetX(), y = GetY();
				GAME_DIR DIR = getCollideDir(blockRect);
				switch (DIR)
				{
				case DIR_LEFT:					
				case DIR_RIGHT:
					startEvent(EVENTTYPE::BULLET_EXPLODE);
					break;
				case DIR_UP:
					x = GetX();
					y = map->GetY() + (row + 1)*map->getTileHeight();		//紧靠障碍下侧
					speedY = -abs(speedY);
					break;
				case DIR_DOWN:
					x = GetX();
					y = map->GetY() + (row)*map->getTileHeight() - GetRatioSize().cy;  //紧靠障碍上侧
					onPlantform = true;
					speedY = abs(speedY);
					break;
				default:
					x = lastX;
					y = lastY;
				}
				// 将角色定位在障碍物边界
				SetPosition(x, y);
			}
		}
	}
	return isCollide;
}
bool PlayerBullet::CollideWith(T_Sprite* target, int distance)
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
		MINION_TYPE type = dynamic_cast<Minion*>(target)->getMinionType();
		if (type == MINION_TYPE::MINION_GOOMBA || type == MINION_TYPE::MINION_KOOPA)
		{
			target->SetDir(GetDir());
			dynamic_cast<Minion*>(target)->startEvent(EVENTTYPE::NPC_DEATH_TURNOVER);
			startEvent(EVENTTYPE::BULLET_EXPLODE);
		}	
	}
	return isCollide;
}
void PlayerBullet::playAnimation()
{
	switch (eventId)
	{
	case BULLET_EXPLODE:
		explode();
		break;
	default:
		inEvent = false;
		eventId = -1;
		break;
	}
}