#include "Player.h"
#include "T_Scene.h"
#include "GameMap.h"
#include "Util.h"
Player::Player(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{
	// ----- role status
	lifeCount = 3;
	isInEnvnt = false;
	eventId = -1;
	playerStatus = PLAYER_NONE;		//角色模式
	starStatus = false;					//是否无敌（星星）状态
	
	//------------------------------------------------------------初始化位置，暂不确定
	X;
	Y;
	speedX = 0;
	speedY = -1;

	// ----- MOVE STATUS
	bMove = false;		//是否水平移动状态
	bSquat = false;		//是否下蹲状态
	bJump = false;		//是否跳跃状态

	// -----JUMP STATUS
	jumpStatus =-1;		//跳跃状态0：上升，1：下降
	isBooting = false;	//是否跳跃加速状态

	// ----- MOVE--------------------------------------------------暂不确定，待初始化	
	friction = 2.0;		//水平摩擦，控制惯性滑行距离
	dir = DIR_RIGHT;		//初始化方向,向右

	// ----- JUMP--------------------------------------------------暂不确定，待初始化
	gravity = 9.8;				//基础重力加速度
	timer = GetTickCount();
	
}


Player::~Player()
{
}
//更新玩家横坐标
void Player::updatePositionX()
{
	Util::myprintf(L"current speedx: %d\n", speedX);
	Util::myprintf(L"current X: %d\n", X);
	if (!bSquat)		//非下蹲状态下可水平移动
	{
		if (!bMove)		//水平静止或惯性滑行状态
		{
			if (bSlide && (currentMaxSpeedX - speedX)*20 + endTime <= GetTickCount()){		//惯性滑行状态，减速
				speedX-=(int)friction;
			
				if (speedX < 0)			//恢复水平静止
				{
					speedX = 0;
					bSlide = false;
				}
			}
			
		}
		else if(bMove)
		{
			if ((100 + 35 * speedX) + startTime <= GetTickCount() && speedX < currentMaxSpeedX)		//加速过程,100ms后加速
			{
				++speedX;
			}
		}

		int ispeedX;
		//根据方向设置速度符号
		if (dir == DIR_LEFT)
			ispeedX = -abs(speedX);
		else if (dir == DIR_RIGHT)
			ispeedX = abs(speedX);

		lastX = X;
		X += ispeedX;
	}
}
//更新玩家纵坐标
void Player::updatePositionY()
{
	gravityEffect();		//重力作用
	lastY = Y;
	Y = Y - speedY;

}
//开始跳跃
void Player::startJump()
{
	timer = GetTickCount();		//记录起跳时间，用于控制加速时间(isBooting)

	bJump = true;		//设置跳跃状态
	jumpStatus = 0;		//设置为上升阶段
	isBooting = true;
	speedY = currentMode->basicJumpSpeedY;	//设置跳跃初始速度
}
//落地
void Player::resetJump()
{
	bJump = false;			//设置跳跃状态
	jumpStatus = -1;		//非跳跃状态
	speedY = 0;				//设置竖直速度置零
	isBooting = false;		//加速状态结束

	//stopMove(false);	//水平静止,有惯性
}
//重力作用
void  Player::gravityEffect()
{
	
	float currentGravity = gravity;		//单击跳跃
	if (jumpStatus == 0 && isBooting)	//长按跳跃
	{
		currentGravity = gravity*0.6f;	//减小重力（等价于提供动力）
	}
	else if (jumpStatus == 1)			//跳跃的下落阶段
	{
		currentGravity = gravity*0.8f;	//重力增大，加快下落
	}		
	//if (bJump)
	{
		if (!isOnPlantform && (GetTickCount() - timer)* currentGravity / 350 > abs(speedY) )
		{
			speedY -= 2;	//四舍五入
		}
		if (speedY < 0)		//速度小于零（向下），设置为下落状态
		{
			jumpStatus = 1;
		}
	}
	Util::myprintf(L"current SpeedY: %d\n", speedY);
	
}
//更新玩家坐标
void Player::updatePosition()
{
	if (bSquat)
	{
		return;
	}	
	updatePositionX();
	updatePositionY();
	CollideWith(T_Scene::getBarrier());	//玩家与障碍层碰撞检测
}
//更新帧图
void Player::updateFrame()
{
	if (dir == DIR_LEFT)
	{
		frameRotate = TRANS_HFLIP_NOROT;
	}
	else if (dir == DIR_RIGHT)
	{
		frameRotate = TRANS_NONE;
	}
	LoopFrame();
}
void Player::update()
{
	if (isBooting && timer +currentMode->maxBootTime <= GetTickCount())	//跳跃加速时间结束
	{
		isBooting = false;
	}
	updatePosition();	//更新玩家坐标
	updateFrame();		//更新帧图
}

//设置正常移动状态，初速度和速度上限
void Player::startMove() {
	startTime = GetTickCount();
	endTime = GetTickCount();
	currentMaxSpeedX = currentMode->maxMoveSpeedX;
	speedX = currentMaxSpeedX;
	bMove = true;
}

//按住shift键，提高加速上限
void Player::startSpeedup() {
	currentMaxSpeedX = currentMode->maxRunSpeedX;
}

//松开shift键，恢复加速上限
//若当前速度大于正常移动速度上限，将速度设置为正常移动上限
void Player::resetSpeedup() {
	currentMaxSpeedX = currentMode->maxMoveSpeedX;
	if (speedX > currentMaxSpeedX)
	{
		speedX = currentMaxSpeedX;
	}
}
//停止水平移动
void Player::stopMove(bool immediately) {
	endTime = GetTickCount();
	
	if (speedX <= 2||immediately)
	{
		speedX = 0;
		bMove = false;
	}
	else
	{
		bMove = false;
		bSlide = true;
	}
}

void Player::initBigRedMode(PLAYERMODE* bigRedMode)
{
	this->bigRedMode = (PLAYERMODE*)malloc(sizeof(PLAYERMODE));
	memcpy(this->bigRedMode, bigRedMode, sizeof(PLAYERMODE));

}
void Player::initNormalMode(PLAYERMODE* normalMode)
{
	this->normalMode = (PLAYERMODE*)malloc(sizeof(PLAYERMODE));
	memcpy(this->normalMode, normalMode, sizeof(PLAYERMODE));
}
void  Player::setPlayerMode(PLAYERSTATUS status)
{
	if (playerStatus == status) {
		return;
	}
	switch (status)
	{
	case PLAYER_NORMAL:
	{		
		if (normalMode == NULL) {
			Util::myprintf(L"Player::setPlayerMode : normalMode == NULL\n");
		}
		currentMode = normalMode;
		playerStatus = status;
		break;
	}
	case PLAYER_BIGRED:
	{
		if (bigRedMode == NULL) {
			Util::myprintf(L"Player::setPlayerMode : bigRedMode == NULL\n");
		}
		currentMode = bigRedMode;
		playerStatus = status;
		break;
	}	
	}
	SetImage(&(currentMode->frameMode.img));
	SetWidth(currentMode->frameMode.frameWidth);
	SetHeight(currentMode->frameMode.frameHeight);

	frameCols = currentMode->frameMode.img.GetImageWidth() / GetWidth();		// 动画帧图片总列数
	frameRows = currentMode->frameMode.img.GetImageHeight() / GetHeight();		// 动画帧图片总行数
	
	rawFrames = frameCols*frameRows;							// 记录原始动画总帧数
	forward = 0;												// 当前帧计数初始化
	backward = totalFrames - 1;

	frameSequence = currentMode->frameMode.runFrmSequence;
	totalFrames = currentMode->frameMode.nRunFrames;			// 动画总帧数
	loopForward = true;

	//恢复静止状态		
	stopMove(true);
	resetJump();
	setSquat(false);
}
void Player::Draw(HDC hdc) {
	int frmIndex = -1;
	
	if (!bMove && !bJump && !bSquat)
	{
		if (bSlide)		//急停帧
		{
			frmIndex = currentMode->frameMode.speedDownFrame;
		}
		else {					//静止帧
			frmIndex = currentMode->frameMode.stopFrame;
		}
	}
	else if (currentMode->canSquat && bSquat)
	{
		frmIndex = currentMode->frameMode.squatHeight;
	}	
	else if (bMove && !bJump)
	{
		frmIndex = frameSequence[forward];		
	}
	else if (bJump)
	{
		frmIndex = 5;
	}		


	Util::myprintf(L"current frame: %d\n",frmIndex);
	if (bSquat)
	{
		spImg.PaintRegion(spImg.GetBmpHandle(),hdc,X,Y,currentMode->frameMode.frameWidth *frmIndex, 
			currentMode->frameMode.frameHeight - currentMode->frameMode.squatHeight,
			currentMode->frameMode.frameHeight, currentMode->frameMode.squatHeight,frameRatio, frameRotate, frameAlpha);
	}
	else if (bJump)
	{
		spImg.PaintRegion(spImg.GetBmpHandle(), hdc, X, Y, Width*frmIndex, 0, Width, Height,
			frameRatio, frameRotate, frameAlpha);
	}
	else
	{
		spImg.PaintFrame(
			spImg.GetBmpHandle(), hdc, (int)X, (int)Y, frmIndex,
			frameCols, Width, Height, frameRatio, frameRotate, frameAlpha
		);
	}
	
}
bool Player::CollideWith(IN T_Map* map)
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

	isOnPlantform = false;
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
				RECT blockRect = { col*map->getTileWidth()+(map->GetX()) ,row*map->getTileHeight()+(map->GetY()),
					(col+1)*map->getTileWidth() + (map->GetX()),(row+1)*map->getTileHeight() + (map->GetY()) };

				int x = GetX(), y = GetY();		
				GAME_DIR DIR = getCollideDir(blockRect);
				switch (DIR)
				{
				case DIR_LEFT:
					x = map->GetX() + (col + 1)*map->getTileWidth();	//紧靠障碍右侧
					y = GetY();
					speedX = 0;
					block = { col ,row ,DIR_RIGHT};		//保存发生碰撞的地图块序列
					collideBlocks.push_back(block);
					break;
				case DIR_RIGHT:
					x = map->GetX() + col*map->getTileWidth() - GetWidth();  //紧靠障碍左侧
					y = GetY();
					speedX = 0;
					block = { col ,row ,DIR_LEFT };		//保存发生碰撞的地图块序列
					collideBlocks.push_back(block);
					break;
				case DIR_UP:
					x = GetX();
					y = map->GetY()+(row + 1)*map->getTileHeight();		//紧靠障碍下侧
					speedY = -speedY;
					block = { col ,row ,DIR_DOWN };		//保存发生碰撞的地图块序列
					collideBlocks.push_back(block);
					break;
				case DIR_DOWN:
					x = GetX();
					y = map->GetY() + row*map->getTileHeight() - GetHeight();  //紧靠障碍上侧
					isOnPlantform = true;
					resetJump();
					block = { col ,row ,DIR_UP };		//保存发生碰撞的地图块序列
					collideBlocks.push_back(block);
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
GAME_DIR Player::getCollideDir(RECT target)
{
	RECT oldRect = *this->GetCollideRect();
	RECT currentRect = *this->GetCollideRect();
	oldRect.left = this->GetCollideRect()->left-(X-lastX);
	oldRect.right = this->GetCollideRect()->right - (X - lastX);
	oldRect.top = this->GetCollideRect()->top -(Y-lastY);
	oldRect.bottom = this->GetCollideRect()->bottom - (Y - lastY);
	
 	if (oldRect.top >= target.bottom && this->GetCollideRect()->top<=target.bottom )
	{
		return DIR_UP;
	}
	if (oldRect.bottom <=target.top && this->GetCollideRect()->bottom >= target.top)
	{
		return DIR_DOWN;
	}
	if (oldRect.left >= target.right && this->GetCollideRect()->left <= target.right)
	{
		return DIR_LEFT;
	}
	if (oldRect.right <= target.left && this->GetCollideRect()->right >= target.left)
	{
		return DIR_RIGHT;
	}
	return DIR_NONE;
}

