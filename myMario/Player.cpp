#include "Player.h"
#include "GameScene.h"
#include "GameMap.h"
#include "Util.h"
Player::Player(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{
	// ----- role status
	lifeCount = 3;
	inEvent = false;
	eventId = -1;
	playerStatus = PLAYER_NONE;		//角色模式
	frameFrequence = 4;
	
	// ---初始化位置
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

	// ----- MOVE
	friction = 1.0;		//水平摩擦，控制惯性滑行距离

	// ----- JUMP
	gravity = 6;				//基础重力加速度
	jumpTimer = GetTickCount();
	
	// -----FIREBALL
	fireballCD = 300;
	fireballTimer = GetTickCount();

	safeTime = 5000;		//变小后的安全时间
	blinkCount = 0;

	bColorful=false;				//是否无敌（星星）状态
	colorfulTime=10000;				//彩色效果时间
	colorMaintain = 0;				//控制单个颜色已保持帧数
	colorMaintainMax = 6;			//控制单个颜色最大保持帧数
	oldColor = PlayerColor;			//彩色效果开始前的颜色
}


Player::~Player()
{
}
//更新玩家横坐标
void Player::updatePositionX()
{

	if (!bSquat)		//非下蹲状态下可水平移动
	{
		if (!bMove)		//水平静止或惯性滑行状态
		{
			if (onPlantform && bSlide && (currentMaxSpeedX - speedX)*100 + (int)(moveTimer*friction) <= (int)(GetTickCount()*friction))
			{
				//惯性滑行状态，减速(空中不减速)				
				speedX -= 1;		
				if (speedX <= 0)			//恢复水平静止
				{
					speedX = 0;
					bSlide = false;
					//若滑行期间尝试改变方向，此时修改方向
					if (dirChanged) {
						dirChanged = false;
						dir = (dir == DIR_LEFT) ?  DIR_RIGHT : DIR_LEFT;
					}
				}
			}
			if (!onPlantform && bSlide && (currentMaxSpeedX - speedX) * 100 + (int)(moveTimer*friction) <= (int)(GetTickCount()*friction))
			{
				
			}
		}
		else if(bMove)
		{
			if (( 200 * speedX) + moveTimer <= GetTickCount() && speedX < currentMaxSpeedX)		//加速过程,每150ms后加速
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
		X += ispeedX;
	}
}
//更新玩家纵坐标
void Player::updatePositionY()
{
	if (isBooting && jumpTimer + currentMode->maxBootTime <= GetTickCount())	//跳跃加速时间结束
	{
		isBooting = false;
	}
	if (onPlantform && jumpStatus == 1)	//如果为下落到平台则解除跳跃状态
	{
		resetJump();
	}
	
	gravityEffect();		//重力作用	
	Y = Y - speedY;
	
	if (Y > T_Scene::getBarrier()->GetHeight())
	{
		this->playerDeath(true);
	}
}
//开始跳跃
void Player::startJump()
{
	jumpTimer = GetTickCount();		//记录起跳时间，用于控制加速时间(isBooting)
	onPlantform = false;
	bJump = true;		//设置跳跃状态
	jumpStatus = 0;		//设置为上升阶段
	isBooting = true;
	startHeight = GetY();	//设置跳跃起始高度
	speedY = currentMode->basicJumpSpeedY;	//设置跳跃初始速度
}
//落地
void Player::resetJump()
{	
	bJump = false;			//设置跳跃状态
	jumpStatus = -1;		//非跳跃状态
	speedY = 0;				//设置竖直速度置零
	isBooting = false;		//加速状态结束		
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
	if (!onPlantform && (GetTickCount() - jumpTimer)* currentGravity / 270 >currentMode->basicJumpSpeedY- abs(speedY) )
	{
		speedY -= 1;	//四舍五入
	}
	if (speedY < 0)		//速度小于零（向下），设置为下落状态
	{
		jumpStatus = 1;
	}
		
	//Util::myprintf(L"current SpeedY: %d\n", speedY);
	
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
}
//更新帧图
void Player::updateFrame()
{
	//朝向控制
	if (dir == DIR_LEFT)
	{
		
		if (bSlide&&dirChanged)		//急停帧
		{
			frameRotate = TRANS_NONE;
		}
		else
		{
			frameRotate = TRANS_HFLIP_NOROT;
		}
	}
	else if (dir == DIR_RIGHT)
	{
		if (bSlide&&dirChanged)		//急停帧
		{
			frameRotate = TRANS_HFLIP_NOROT;
		}
		else
		{
			frameRotate = TRANS_NONE;
		}
	}

	//帧图选择
	if (!bMove && !bJump && !bSquat)
	{
		if (bSlide&&dirChanged)		//急停帧
		{
			currentFrmIndex = currentFrame->speedDownFrame;
		}
		else {					//静止帧
			currentFrmIndex = currentFrame->stopFrame;
		}
	}
	if (currentMode->canSquat && bSquat)
	{
		currentFrmIndex = currentFrame->squatFrame;
	}
	if ((bMove && !bJump) || (bSlide && !dirChanged))
	{
		//控制玩家帧速率
		if (bSpeedUp)
		{
			frameFrequence = 2;
		}
		else
		{
			frameFrequence = 4;
		}
		LoopFrame(frameFrequence);
		currentFrmIndex = frameSequence[forward];
	}
	if (bJump)
	{
		currentFrmIndex = currentFrame->jumpFrame;
	}	
	if (onCreateFireBall)
	{
		currentFrmIndex = currentFrame->fireBallFrame;
		//从发射炮弹状态恢复
		if (fireballTimer+200<GetTickCount())
		{
			onCreateFireBall = false;
		}
	}
	//安全时间闪烁提示
	if (bSafe)
	{
		if (blinkCount > 8)
		{
			int alpha = (GetAlpha() == 255) ? 150 : 255;
			SetAlpha(alpha);
			blinkCount = 0;
		}	
		else
		{
			blinkCount++;
		}
		if (GetTickCount() >= startTime + safeTime)
		{
			SetAlpha(255);
			bSafe = false;
		}
	}
	//颜色选择
	if (bColorful)
	{
		if (colorMaintain > colorMaintainMax)
		{
			PlayerColor++;
			if (PlayerColor > currentFrame->totalColor-1)
			{
				PlayerColor = 0;
			}
			colorMaintain = 0;
		}
		else
		{
			colorMaintain++;
		}
		if (GetTickCount() >= startTime + colorfulTime)
		{
			PlayerColor = oldColor;
			bColorful = false;
		}
	}
	currentFrmIndex += PlayerColor*frameCols;

	//Util::myprintf(L"currentFrmIndex: %d-------------------------------------\n", currentFrmIndex);
}
void Player::update()
{
	if (!inEvent)
	{
		if (checkOnplantForm(T_Scene::getBarrier()) || checkOnplantForm(T_Scene::getNormalBrick()) || checkOnplantForm(T_Scene::getPropBrick()))
			onPlantform = true;
		else
			onPlantform = false;
		updatePosition();	//更新玩家坐标
		CollideWith(T_Scene::getBarrier());		//玩家与障碍层碰撞检测
		CollideWith(T_Scene::getNormalBrick());	//玩家与砖层碰撞检测
		CollideWith(T_Scene::getPropBrick());	//玩家与砖层碰撞检测
		updateFrame();		//更新帧图
	}
	else
	{
		playAnimation();
	}
	
}

//设置正常移动状态，初速度和速度上限
void Player::startMove() {
	moveTimer = GetTickCount();

	currentMaxSpeedX = currentMode->maxMoveSpeedX;
	speedX = 0;
	bMove = true;
}

//按住shift键，提高加速上限
void Player::startSpeedup() {
	currentMaxSpeedX = currentMode->maxRunSpeedX;
	bSpeedUp = true;
}

//松开shift键，恢复加速上限
//若当前速度大于正常移动速度上限，将速度设置为正常移动上限
void Player::resetSpeedup() {
	currentMaxSpeedX = currentMode->maxMoveSpeedX;
	bSpeedUp = false;
	if (speedX > currentMaxSpeedX)
	{
		speedX = currentMaxSpeedX;
	}
}
//停止水平移动
void Player::stopMove(bool immediately) {
	moveTimer = GetTickCount();
	if (immediately)
	{
		speedX = 0;
		bMove = false;
	}
	else
	{
		if (!bJump)
		{
			if (speedX <= 3)
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
		else
		{
			if (speedX > 0)
			{			
				bMove = false;
				bSlide = true;
			}
			else
			{
				bMove = false;
				speedX = 0;
			}
		}
	}
	
}

//发射炮弹
void  Player::createFireBall()
{
	if (currentMode->canCreateFireBall&& fireballTimer + fireballCD <= GetTickCount())	//子弹冷却时间控制
	{
		int bulletX, bulletY;
		if (dir == DIR_LEFT)
		{
			bulletX = X -24;
		}
		else
		{
			bulletX = X + GetRatioSize().cx-8;
		}
		bulletY = Y + GetRatioSize().cy / 4-8;
		GameScene::getInstance()->appendMinion(MINION_TYPE::MINION_PLAYERBULLET,bulletX, bulletY, GetDir());
		fireballTimer = GetTickCount();
		onCreateFireBall = true;
	}	
}

void Player::initPlayerMode(PLAYERSTATUS state,PLAYERMODE* playerMode)
{
	switch (state)
	{
	case PLAYER_NORMAL:
		this->normalMode = (PLAYERMODE*)malloc(sizeof(PLAYERMODE));
		memcpy(this->normalMode, playerMode, sizeof(PLAYERMODE));
		break;
	case PLAYER_BIGNORMAL:
		this->bigNormalMode = (PLAYERMODE*)malloc(sizeof(PLAYERMODE));
		memcpy(this->bigNormalMode, playerMode, sizeof(PLAYERMODE));
		break;
	case PLAYER_BIGRED:
		this->bigRedMode = (PLAYERMODE*)malloc(sizeof(PLAYERMODE));
		memcpy(this->bigRedMode, playerMode, sizeof(PLAYERMODE));
		break;
	default:
		break;
	}
}

//初始化帧模式，frameType：0小型角色，1大型角色
void Player::initFrameMode(int frameType,PLAYERFRAME* playerFrame)
{
	switch (frameType)
	{
	case 0:
		this->smallFrame = (PLAYERFRAME*)malloc(sizeof(PLAYERFRAME));
		memcpy(this->smallFrame, playerFrame, sizeof(PLAYERFRAME));
		break;
	case 1:
		this->bigFrame = (PLAYERFRAME*)malloc(sizeof(PLAYERFRAME));
		memcpy(this->bigFrame, playerFrame, sizeof(PLAYERFRAME));
		break;
	}
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
		currentFrame = smallFrame;
		PlayerColor = currentFrame->initColor;		//选择颜色
		playerStatus = status;
		break;
	}
	case PLAYER_BIGNORMAL:
	{
		if (bigNormalMode == NULL) {
			Util::myprintf(L"Player::setPlayerMode : bigNormalMode == NULL\n");
		}
		currentMode = bigNormalMode;
		currentFrame = bigFrame;
		PlayerColor = currentFrame->initColor;		//选择颜色
		playerStatus = status;
		break;
	}	
	case PLAYER_BIGRED:
	{
		if (bigRedMode == NULL) {
			Util::myprintf(L"Player::setPlayerMode : bigRedMode == NULL\n");
		}
		currentMode = bigRedMode;
		currentFrame = bigFrame;
		PlayerColor = 10;		//选择颜色
		playerStatus = status;
		break;
	}
	}

	//更新T_Sprite属性
	SetImage(&(currentFrame->img));
	SetWidth(currentFrame->frameWidth);
	SetHeight(currentFrame->frameHeight);
	colideWidth = GetRatioSize().cx;
	colideHeight = GetRatioSize().cy;
	

	frameCols = currentFrame->img.GetImageWidth() / GetWidth();		// 动画帧图片总列数
	frameRows = currentFrame->img.GetImageHeight() / GetHeight();		// 动画帧图片总行数
	
	rawFrames = frameCols*frameRows;							// 记录原始动画总帧数
	forward = 0;												// 当前帧计数初始化
	backward = totalFrames - 1;

	//初始化运动帧
	SetSequence(currentFrame->runFrmSequence, currentFrame->nRunFrames);
	loopForward = true;
	
	//恢复静止状态		
	stopMove(true);
	resetJump();
	setSquat(false);
}
void Player::Draw(HDC hdc) {
	lastX = X;
	lastY = Y;

	Util::myprintf(L"player x: %d,player y: %d\n",X-GameScene::getInstance()->getSceneX(),Y - GameScene::getInstance()->getSceneY());
	if (bSquat)
	{
		spImg.PaintRegion(spImg.GetBmpHandle(),hdc,X,Y, currentFrame->frameWidth *(currentFrmIndex%frameCols),
			currentFrame->frameHeight - currentFrame->squatHeight,
			currentFrame->frameWidth, currentFrame->squatHeight,frameRatio, frameRotate, frameAlpha);
	}
	else
	{
		spImg.PaintFrame(
			spImg.GetBmpHandle(), hdc, (int)X, (int)Y, currentFrmIndex,
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

	//地图左右边界碰撞
	if (this->X <= mapLeft)
	{
		this->X = mapLeft;
	}
	if (this->X+this->GetRatioSize().cx >= mapRight)
	{
		this->X = mapRight - this->GetRatioSize().cx;
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
	if (speedY <= 0)
	{
		for (int row = startRow; row <= endRow; ++row)
		{
			if (dir == DIR_RIGHT)
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
							speedX = 0;
							block = { col ,row ,DIR_RIGHT };		//保存发生碰撞的地图块序列
																	//collideBlocks.push_back(block);
							break;
						case DIR_RIGHT:
							x = map->GetX() + col*map->getTileWidth() - GetRatioSize().cx;
							y = GetY();
							speedX = 0;
							block = { col ,row ,DIR_LEFT };		//保存发生碰撞的地图块序列
																//collideBlocks.push_back(block);
							break;
						case DIR_UP:
							x = GetX();
							y = map->GetY() + (row + 1)*map->getTileHeight();		//紧靠障碍下侧
							speedY = -abs(speedY);
							block = { col ,row ,DIR_DOWN };
							//保存发生碰撞的地图块序列
							collideBlocks.push_back(block);
							break;
						case DIR_DOWN:
							x = GetX();
							y = map->GetY() + (row)*map->getTileHeight() - GetRatioSize().cy;  //紧靠障碍上侧
							onPlantform = true;
							resetJump();
							block = { col ,row ,DIR_UP };		//保存发生碰撞的地图块序列
							//collideBlocks.push_back(block);
							break;
						}
						// 将角色定位在障碍物边界
						SetPosition(x, y);
					}
				}
			}
			else if (dir == DIR_LEFT)
			{
				for (int col = endCol; col >= startCol; --col)
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
							speedX = 0;
							block = { col ,row ,DIR_RIGHT };		//保存发生碰撞的地图块序列
																	//collideBlocks.push_back(block);
							break;
						case DIR_RIGHT:
							x = map->GetX() + col*map->getTileWidth() - GetRatioSize().cx;
							y = GetY();
							speedX = 0;
							block = { col ,row ,DIR_LEFT };		//保存发生碰撞的地图块序列
																//collideBlocks.push_back(block);
							break;
						case DIR_UP:
							x = GetX();
							y = map->GetY() + (row + 1)*map->getTileHeight();		//紧靠障碍下侧
							speedY = -abs(speedY);
							block = { col ,row ,DIR_DOWN };
							Util::myprintf(L"player collide top \n");
							//保存发生碰撞的地图块序列
							collideBlocks.push_back(block);
							break;
						case DIR_DOWN:
							x = GetX();
							y = map->GetY() + (row)*map->getTileHeight() - GetRatioSize().cy;  //紧靠障碍上侧
							onPlantform = true;
							resetJump();
							block = { col ,row ,DIR_UP };		//保存发生碰撞的地图块序列
							Util::myprintf(L"player collide down \n");
							//collideBlocks.push_back(block);
							break;
						}
						// 将角色定位在障碍物边界
						SetPosition(x, y);
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
							speedX = 0;
							block = { col ,row ,DIR_RIGHT };		//保存发生碰撞的地图块序列
																	//collideBlocks.push_back(block);
							break;
						case DIR_RIGHT:
							x = map->GetX() + col*map->getTileWidth() - GetRatioSize().cx;
							y = GetY();
							speedX = 0;
							block = { col ,row ,DIR_LEFT };		//保存发生碰撞的地图块序列
																//collideBlocks.push_back(block);
							break;
						case DIR_UP:
							x = GetX();
							y = map->GetY() + (row + 1)*map->getTileHeight();		//紧靠障碍下侧
							speedY = -abs(speedY);
							block = { col ,row ,DIR_DOWN };
							//保存发生碰撞的地图块序列
							collideBlocks.push_back(block);
							break;
						case DIR_DOWN:
							x = GetX();
							y = map->GetY() + (row)*map->getTileHeight() - GetRatioSize().cy;  //紧靠障碍上侧
							onPlantform = true;
							resetJump();
							block = { col ,row ,DIR_UP };		//保存发生碰撞的地图块序列
							//collideBlocks.push_back(block);
							break;
						}
						// 将角色定位在障碍物边界
						SetPosition(x, y);
					}
				}
			}
			else if (dir == DIR_LEFT)
			{
				for (int col = endCol; col >= startCol; --col)
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
							speedX = 0;
							block = { col ,row ,DIR_RIGHT };		//保存发生碰撞的地图块序列
																	//collideBlocks.push_back(block);
							break;
						case DIR_RIGHT:
							x = map->GetX() + col*map->getTileWidth() - GetRatioSize().cx;
							y = GetY();
							speedX = 0;
							block = { col ,row ,DIR_LEFT };		//保存发生碰撞的地图块序列
																//collideBlocks.push_back(block);
							break;
						case DIR_UP:
							x = GetX();
							y = map->GetY() + (row + 1)*map->getTileHeight();		//紧靠障碍下侧
							speedY = -abs(speedY);
							block = { col ,row ,DIR_DOWN };
							Util::myprintf(L"player collide top \n");
							//保存发生碰撞的地图块序列
							collideBlocks.push_back(block);
							break;
						case DIR_DOWN:
							x = GetX();
							y = map->GetY() + (row)*map->getTileHeight() - GetRatioSize().cy;  //紧靠障碍上侧
							onPlantform = true;
							resetJump();
							block = { col ,row ,DIR_UP };		//保存发生碰撞的地图块序列
							Util::myprintf(L"player collide down \n");
							//collideBlocks.push_back(block);
							break;
						}
						// 将角色定位在障碍物边界
						SetPosition(x, y);
					}
				}
			}
		}
	}
	
	(dynamic_cast<GameMap*>(map))->setCollideBlocks(collideBlocks); //刷新碰撞地图块
	return isCollide;
}
bool Player::checkOnplantForm(T_Map* map)
{
	if (jumpStatus == 0) {
		onPlantform = false;
		return onPlantform;
	}
	if ((Y + GetRatioSize().cy - map->GetY()) % map->getTileHeight() < 2)
	{
		//玩家下方的地图块
		int row = (Y + GetRatioSize().cy - map->GetY()) / map->getTileHeight();		
		int startCol = (X - map->GetX()+1) / map->getTileWidth();
		int endCol = (X + GetRatioSize().cx - map->GetX()-1) / map->getTileWidth();

		//是否超出地图范围
		if (row > map->getMapRows()-1 || row < 0|| startCol<0|| endCol>map->getMapCols()-1)
		{
			onPlantform = false;
			return onPlantform;
		}
		//检查玩家下方的地图块
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

void Player::startEvent(int eventId)
{	
	this->eventId = eventId;
	inEvent = true;
	currentStep = 0;
	eventTimer = GetTickCount();
	
	//解除运动状态
	onPlantform = false;
	stopMove(true);
	resetJump();
}
void Player::playAnimation()
{
	switch (eventId)
	{
	case PLAYER_DEATH:
		deathAnimation();
		break;
	case PLAYER_LEVELUP:
		levelUpAnimation();
		break;
	case PLAYER_LEVELDOWN:
		levelDownAnimation();
		break;
	case PLAYER_AFTERPOLE:
		break;
	default:
		inEvent = false;
		eventId = -1;
		break;
	}
}

void Player::deathAnimation()
{
	currentFrmIndex = currentFrame->deathFrame;	//设置死亡帧
	switch (currentStep)
	{
	case 0:
		if (eventTimer + 500 <= GetTickCount())		//静止0.5秒
		{
			++currentStep;
		}
		break;
	case 1:
		speedY = 15;	
		++currentStep;
		break;
	case 2:
		updatePosition();
		if (Y - GetRatioSize().cy >= T_Scene::getBarrier()->GetHeight())
		{
			++currentStep;
		}
		break;
	default:
		playerDeath(true);		//设置死亡状态
		break;
	}
}
void Player::levelUpAnimation()
{	
	switch (currentStep)
	{
	case 0:
		active = false;		
		//检测碰撞（防止变大后嵌入到障碍物中）		
		if ((GetCollideRect()->left - GameScene::getInstance()->getSceneX()) % GameScene::getInstance()->getBarrier()->getTileWidth() != 0)
		{
			if (CollideWith(GameScene::getInstance()->getBarrier())||
				CollideWith(GameScene::getInstance()->getNormalBrick())||
					CollideWith(GameScene::getInstance()->getPropBrick()))
			{
				X -= (GetCollideRect()->right) % GameScene::getInstance()->getBarrier()->getTileWidth();
			}
		}
		
		Y -= bigFrame->frameHeight - smallFrame->frameHeight;	//调整玩家高度（不同状态帧图存在高度差，以玩家下边界为基准）
		SetSequence(currentFrame->levelUpFrmSequence , currentFrame->nlevelUpFrames);
		forward = 0;		
		SetAlpha(200);
		++currentStep;
		break;
	case 1:
		if (eventTimer + 1200 >= GetTickCount())	//控制动画播放时间1200ms
		{
			LoopFrame(6);
			currentFrmIndex = frameSequence[forward];
		}
		else
		{
			currentFrmIndex = currentFrame->stopFrame;
			++currentStep;
		}
		break;
	default:
		//动画结束
		active = true;
		SetAlpha(255);
		SetSequence(currentFrame->runFrmSequence, currentFrame->nRunFrames);
		forward = 0;
		stopEvent();
		break;
	}
}
void Player::levelDownAnimation()
{
	switch (currentStep)
	{
	case 0:		
		active = false;
		SetSequence(currentFrame->levelUpFrmSequence, currentFrame->nlevelUpFrames);
		forward = 0;
		SetAlpha(200);
		++currentStep;
		break;
	case 1:
		if (eventTimer + 1200 >= GetTickCount())	//控制动画播放时间1200ms
		{
			LoopFrame(6);
			currentFrmIndex = frameSequence[forward];
		}
		else
		{
			currentFrmIndex = currentFrame->stopFrame;
			++currentStep;
		}
		break;
	default:
		//动画结束
		active = true;
		bSafe = true;
		startTime = GetTickCount();	//安全时间开始计时
		SetAlpha(255);
		Y += bigFrame->frameHeight - Height;	//调整玩家高度（不同状态帧图存在高度差，以玩家下边界为基准）
		setPlayerMode(PLAYER_NORMAL);
		SetSequence(currentFrame->runFrmSequence, currentFrame->nRunFrames);
		forward = 0;
		stopEvent();
		break;
	}
}
void Player::playerDeath(bool immediately)
{
	if (immediately)
	{
		dead = true;
		if(lifeCount>0)
		{ 
			lifeCount--;
		}
	}
	else
	{
		active = false;
		startEvent(PLAYER_DEATH);
	}
}