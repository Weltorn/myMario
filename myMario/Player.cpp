#include "Player.h"
#include "T_Scene.h"
#include "Util.h"
Player::Player(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{
	// ----- role status
	lifeCount = 3;
	isInEnvnt = false;
	eventId = -1;
	playerStatus = PLAYER_NORMAL;		//角色模式
	starStatus = false;					//是否无敌（星星）状态
	
	//------------------------------------------------------------初始化位置，暂不确定
	X;
	Y;
	speedX = 0;
	speedY = 0;

	// ----- MOVE STATUS
	bMove = false;		//是否水平移动状态
	bSquat = false;		//是否下蹲状态
	bJump = false;		//是否跳跃状态

	// -----JUMP STATUS
	jumpStatus =-1;		//跳跃状态0：上升，1：下降
	isBooting = false;	//是否跳跃加速状态

	// ----- MOVE--------------------------------------------------暂不确定，待初始化	
	friction = 1.0;		//水平摩擦，控制惯性滑行距离
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
	Util::myprintf(L"current speedx: %d\n",speedX);
	if (!bSquat)		//非下蹲状态下可水平移动
	{
		if (!bMove)		//水平静止或惯性滑行状态
		{
			if (speedX > 0 && (currentMaxSpeedX - speedX)*80 <= GetTickCount() - endTime){		//惯性滑行状态，减速
				speedX-=(int)friction;
			
				if (speedX < 0)			//恢复水平静止
				{
					speedX = 0;
				}
			}
			
		}
		else if(bMove)
		{
			if ((100 + 35 * speedX) <= GetTickCount()- startTime && speedX < currentMaxSpeedX)		//加速过程,100ms后加速
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
	
	if (Y <= 500) {
		Y = Y - speedY;
	}
	if(Y >= 500)
	{
		Y = 500;
		resetJump();
	}
	Util::myprintf(L"current SpeedY: %d\n",speedY);
	if (speedY < 0)		//速度小于零（向下），设置为下落状态
	{
		//bJump = true;		//行走下落和跳跃下落，处理方式相同
		jumpStatus = 1;
	}
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
		currentGravity = gravity*1.0f;		//重力增大，加快下落
	}

	speedY = (int)round(currentMode->basicJumpSpeedY - (float)(GetTickCount()-timer)* currentGravity / 500);	//四舍五入
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
	//CollideWith(T_Scene::getBarrier());	//玩家与障碍层碰撞检测
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
	if (immediately)
	{
		speedX = 0;
		bMove = false;
	}
	else
	{
		bMove = false;
	}
}

bool Player::CollideWith(IN T_Map* map)
{
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

	// 根据角色矩形上、下、左、右的矩形区域判断哪个矩形区域为障碍
	for (int row = startRow; row <= endRow; ++row)
	{
		for (int col = startCol; col <= endCol; ++col)
		{
			// 如果当前矩形所在的图块在地图数据中为非0，就表示是障碍
			if (map->getTile(col, row) != 0)
			{
				mapBlockPT.x = col;	// 记录当前障碍图块的列
				mapBlockPT.y = row;	// 记录当前障碍图块的行

				// 根据角色当前的方向计算碰撞前的位置
				int x = GetX(), y = GetY();
				switch (GetDir())
				{
				case DIR_LEFT:
					x = (col + 1)*map->getTileWidth() + 1;	//紧靠障碍右侧
					y = GetY();
					break;
				case DIR_RIGHT:
					x = col*map->getTileWidth() - GetWidth() - 1;  //紧靠障碍左侧
					y = GetY();
					break;
				case DIR_UP:
					x = GetX();
					y = (row + 1)*map->getTileHeight() + 1;		//紧靠障碍下侧
					break;
				case DIR_DOWN:
					x = GetX();
					y = row*map->getTileHeight() - GetHeight() - 1;  //紧靠障碍上侧
					break;
				}
				// 将角色定位在障碍物边界
				SetPosition(x, y);
				return true;
			}
		}
	}
	return false;

}
void Player::initBigRedMode(PLAYERMODE* bigRedMode)
{
	this->bigRedMode = (PLAYERMODE*)malloc(sizeof(PLAYERMODE));
	memcpy(this->bigRedMode, bigRedMode, sizeof(PLAYERMODE));

}
void Player::initNormaldMode(PLAYERMODE* normalMode)
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
		break;
	}
	case PLAYER_BIGREDGER:
	{
		if (bigRedMode == NULL) {
			Util::myprintf(L"Player::setPlayerMode : bigRedMode == NULL\n");
		}
		currentMode = bigRedMode;
		break;
	}	
	}
	SetImage(&(currentMode->frameMode->img));
	SetWidth(currentMode->frameMode->frameWidth);
	SetHeight(currentMode->frameMode->frameHeight);

	frameCols = currentMode->frameMode->img.GetImageWidth() / GetWidth();		// 动画帧图片总列数
	frameRows = currentMode->frameMode->img.GetImageHeight() / GetHeight();		// 动画帧图片总行数
	
	rawFrames = frameCols*frameRows;							// 记录原始动画总帧数
	forward = 0;												// 当前帧计数初始化
	backward = totalFrames - 1;

	frameSequence = currentMode->frameMode->runFrmSequence;
	totalFrames = currentMode->frameMode->nRunFrames;			// 动画总帧数
	loopForward = true;

	//恢复静止状态		
	stopMove(true);
	resetJump();
	setSquat(false);
}
void Player::Draw(HDC hdc) {
	int frmIndex;
	
	if (!bMove && !bJump && !bSquat)
	{
		if (speedX != 0)		//急停帧
		{
			frmIndex = currentMode->frameMode->speedDownFrame;
		}
		else {					//静止帧
			frmIndex = currentMode->frameMode->stopFrame;
		}
	}
	else if (currentMode->canSquat && bSquat)
	{
		frmIndex = currentMode->frameMode->squatHeight;
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
		spImg.PaintRegion(spImg.GetBmpHandle(),hdc,X,Y,currentMode->frameMode->frameWidth *frmIndex, 
			currentMode->frameMode->frameHeight - currentMode->frameMode->squatHeight,
			currentMode->frameMode->frameHeight, currentMode->frameMode->squatHeight,frameRatio, frameRotate, frameAlpha);
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

