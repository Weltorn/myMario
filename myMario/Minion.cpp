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

// 判断怪物与目标矩形的碰撞方向（相对于怪物的方向）
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

	COLLIDBLOCKS collideBlocks;
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

				COLLIDBLOCK block;

				//碰撞的地图块
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
					y = map->GetY() + (row + 1)*map->getTileHeight();		//紧靠障碍下侧
					speedY = -abs(speedY);					
					break;
				case DIR_DOWN:
					x = GetX();
					y = map->GetY() + (row)*map->getTileHeight() - GetRatioSize().cy;  //紧靠障碍上侧
					onPlantform = true;
					speedY = 0;					
					break;
				}
				// 将角色定位在障碍物边界
				SetPosition(x, y);
			}
		}
	}
	(dynamic_cast<GameMap*>(map))->setCollideBlocks(collideBlocks); //刷新碰撞地图块
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
//竖直移动
void Minion::updatePositionY()
{
	gravityEffect();		//重力作用
	lastY = Y;
	Y = Y - speedY;
}
//水平移动						
void Minion::updatePositionX()
{
	int ispeedX;
	//根据方向设置速度符号
	if (dir == DIR_LEFT)
		ispeedX = -abs(speedX);
	else if (dir == DIR_RIGHT)
		ispeedX = abs(speedX);

	lastX = X;
	X += ispeedX;
}
//更新帧图
void Minion::updateFrame()
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

	//帧图选择
	LoopFrame();
	currentFrmIndex = frameSequence[forward];


}
void Minion::update()
{
	if (!inEvent)
	{
		checkOnplantForm(T_Scene::getBarrier());
		if (onPlantform)		//在平台上，竖直速度为零
		{
			speedY = 0;
		}
		updatePosition();					//更新坐标
		CollideWith(T_Scene::getBarrier());	//障碍层碰撞检测
		CollideWith(T_Scene::getPlayer());	//玩家层碰撞检测
		updateFrame();						//更新帧图
	}
	else
	{
		playAnimation();
	}
}

//检查是否在平台上
bool Minion::checkOnplantForm(T_Map* map)
{	
	if ((Y + GetRatioSize().cy - map->GetY()) % map->getTileHeight() < 2)
	{
		//怪物下方的地图块
		int row = (Y + GetRatioSize().cy - map->GetY()) / map->getTileHeight();
		int startCol = (X - map->GetX() + 1) / map->getTileWidth();
		int endCol = (X + GetRatioSize().cx - map->GetX() - 1) / map->getTileWidth();

		//是否超出地图范围
		if (row > map->getMapRows() - 1 || row < 0 || startCol<0 || endCol>map->getMapCols() - 1)
		{
			onPlantform = false;
			return onPlantform;
		}
		//检查怪物下方的地图块
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

//重力作用
void  Minion::gravityEffect()
{
	float currentGravity = gravity;		//单击跳跃	
	
	if (!onPlantform && (GetTickCount() - timer)* currentGravity / 270 >abs(speedY))
	{
		speedY -= 1;	
	}	
	if (speedY == 0)		//更新计时器
	{
		timer = GetTickCount();
	}
}


// 开始事件
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

//播放事件动画
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
