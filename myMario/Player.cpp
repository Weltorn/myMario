#include "Player.h"
#include "T_Scene.h"
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
	starStatus = false;					//是否无敌（星星）状态
	
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
	
}


Player::~Player()
{
}
//更新玩家横坐标
void Player::updatePositionX()
{
	Util::myprintf(L"current speedx: %d\n", speedX);

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
			if (( 150 * speedX) + moveTimer <= GetTickCount() && speedX < currentMaxSpeedX)		//加速过程,每150ms后加速
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
	/*if (speedY <= -12)
	{
		currentGravity = 0.0;
	}
	*/
		if (!onPlantform && (GetTickCount() - jumpTimer)* currentGravity / 270 >currentMode->basicJumpSpeedY- abs(speedY) )
		{
			speedY -= 1;	//四舍五入
		}
		if (speedY < 0)		//速度小于零（向下），设置为下落状态
		{
			jumpStatus = 1;
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
	if (checkOnplantForm(T_Scene::getBarrier()))
	{
		if (jumpStatus == 1)	//如果为下落状态则解除跳跃状态
		{
			resetJump();
		}			
		Util::myprintf(L" on plantform now!-----------------------------------------------------\n");
	}
	updatePositionX();
	updatePositionY();
	CollideWith(T_Scene::getBarrier());	//玩家与障碍层碰撞检测
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
			currentFrmIndex = currentMode->frameMode.speedDownFrame;
		}
		else {					//静止帧
			currentFrmIndex = currentMode->frameMode.stopFrame;
		}
	}
	if (currentMode->canSquat && bSquat)
	{
		currentFrmIndex = currentMode->frameMode.squatFrame;
	}
	if ((bMove && !bJump) || (bSlide && !dirChanged))
	{
		LoopFrame();
		currentFrmIndex = frameSequence[forward];
	}
	if (bJump)
	{
		currentFrmIndex = currentMode->frameMode.jumpFrame;
	}	
}
void Player::update()
{
	if (isBooting && jumpTimer +currentMode->maxBootTime <= GetTickCount())	//跳跃加速时间结束
	{
		isBooting = false;
	}
	updatePosition();	//更新玩家坐标
	updateFrame();		//更新帧图
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

	//更新T_Sprite属性
	SetImage(&(currentMode->frameMode.img));
	SetWidth(currentMode->frameMode.frameWidth);
	SetHeight(currentMode->frameMode.frameHeight);
	colideWidth = GetRatioSize().cx;
	colideHeight = GetRatioSize().cy;

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
	
	if (bSquat)
	{
		spImg.PaintRegion(spImg.GetBmpHandle(),hdc,X,Y,currentMode->frameMode.frameWidth *currentFrmIndex,
			currentMode->frameMode.frameHeight - currentMode->frameMode.squatHeight,
			currentMode->frameMode.frameWidth, currentMode->frameMode.squatHeight,frameRatio, frameRotate, frameAlpha);
	}
	else if (bJump)
	{
		spImg.PaintRegion(spImg.GetBmpHandle(), hdc, X, Y, Width*currentFrmIndex, 0, Width, Height,
			frameRatio, frameRotate, frameAlpha);
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
		this->X = mapRight - this->GetRatioSize().cx-1;
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
				RECT blockRect = { col*map->getTileWidth()+(map->GetX()) ,row*map->getTileHeight()+(map->GetY()),
					(col+1)*map->getTileWidth() + (map->GetX()),(row+1)*map->getTileHeight() + (map->GetY()) };

				int x = GetX(), y = GetY();		
				GAME_DIR DIR = getCollideDir(blockRect);
				
				switch (DIR)
				{
				case DIR_LEFT:
					x = map->GetX() + (col+1)*map->getTileWidth();
					y = GetY();
					speedX = 0;					
					block = { col ,row ,DIR_RIGHT};		//保存发生碰撞的地图块序列
					collideBlocks.push_back(block);
					break;
				case DIR_RIGHT:					
					x = map->GetX() + col*map->getTileWidth()-GetRatioSize().cx;
					y = GetY();
					speedX = 0;				
					block = { col ,row ,DIR_LEFT };		//保存发生碰撞的地图块序列
					collideBlocks.push_back(block);
					break;
				case DIR_UP:
					x = GetX();
					y = map->GetY()+(row + 1)*map->getTileHeight();		//紧靠障碍下侧
					speedY = -abs(speedY);
					block = { col ,row ,DIR_DOWN };	
					//保存发生碰撞的地图块序列
					collideBlocks.push_back(block);
					break;
				case DIR_DOWN:
					x = GetX();
					y = map->GetY() + row*map->getTileHeight() - GetRatioSize().cy;  //紧靠障碍上侧
					onPlantform = true;			
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
	
	Util::myprintf(L" lastposX: %d,posX: %d\n,target.right: %d\n", oldRect.right, this->GetCollideRect()->right, target.left);
	if (oldRect.left >= target.right && this->GetCollideRect()->left <= target.right)
	{
		return DIR_LEFT;
	}
	if (oldRect.right <= target.left && this->GetCollideRect()->right >= target.left)
	{
		return DIR_RIGHT;
	}
 	if (oldRect.top >= target.bottom && this->GetCollideRect()->top<=target.bottom )
	{
		return DIR_UP;
	}
	if (oldRect.bottom <=target.top && this->GetCollideRect()->bottom >= target.top)
	{
		return DIR_DOWN;
	}	
	return DIR_NONE;
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

void Player::loadEvents()
{
	//加载死亡事件
	EVENTSTEP step1 = {true,false,false,500,NULL};//变成死亡状态帧，静止
	deathEvent.push_back(step1);
	EVENTSTEP step2 = { false,true,true,0,{0,42} };//上升
	deathEvent.push_back(step2);
	EVENTSTEP step3 = { false,false,false,0,NULL };//下降,超出边界时自动判定死亡
	deathEvent.push_back(step3);
}
void Player::startEvent(int eventId)
{
	switch (eventId)
	{
	case PLAYER_DEATH:
		currentEvent = &deathEvent;
		break;
	case PLAYER_LEVELUP:
		currentEvent = &levelUpEvent;
		break;
	case PLAYER_LEVELDOWN:
		currentEvent = &levelDownEvent;
		break;
	case PLAYER_AFTERPOLE:
		currentEvent = &afterPoleEvent;
		break;
	default:
		currentEvent = NULL;
		break;
	}
	this->eventId = eventId;
	currentStep = 0;
	inEvent = true;
	eventTimer = GetTickCount();
	posBeforeEvent = {X-T_Scene::getBarrier()->GetX() ,Y-T_Scene::getBarrier()->GetY() };//当前相对地图位置
}
bool Player::checkNextStep()
{
	bool stepEnd = false;
	if ((*currentEvent)[currentStep].useTime)	//检查时间条件
	{
		if (eventTimer + (*currentEvent)[currentStep].lastTime >= GetTickCount())
		{
			stepEnd = true;
		}
		else
		{
			stepEnd = false;
		}
	}
	if ((*currentEvent)[currentStep].usePos)	//检查位置条件
	{
		int x = X - T_Scene::getBarrier()->GetX();
		int y = Y - T_Scene::getBarrier()->GetY();
		if ((*currentEvent)[currentStep].relativePos)
		{
			if (abs(x - posBeforeEvent.x) >= (*currentEvent)[currentStep].endPt.x &&
				(x - posBeforeEvent.x) ^ (*currentEvent)[currentStep].endPt.x >= 0 &&
				abs(y - posBeforeEvent.y) >= (*currentEvent)[currentStep].endPt.y &&
				(y - posBeforeEvent.y) ^ (*currentEvent)[currentStep].endPt.y >= 0)		//同号，且绝对值大于endPt
				stepEnd = true;
			else
				stepEnd = false;
		}
		else
		{
			if (x  >= (*currentEvent)[currentStep].endPt.x &&				
				y  >= (*currentEvent)[currentStep].endPt.y )		//到达指定位置
				stepEnd = true;
			else
				stepEnd = false;
		}
	}
	if (stepEnd)
	{
		if (currentStep == currentEvent->size() - 1)		//判断是否结束事件
		{
			endEvent(eventId);
		}
		else
		{
			++currentStep;		//进入下一事件步骤
		}
	}
	return stepEnd;
}
void Player::playAnimation()
{
	switch (eventId)
	{
	case PLAYER_DEATH:
		switch (currentStep)
		{
		case 0:

		default:
			break;
		}
		break;
	case PLAYER_LEVELUP:
		break;
	case PLAYER_LEVELDOWN:
		break;
	case PLAYER_AFTERPOLE:
		break;
	default:
		break;
	}
}
void Player::endEvent(int eventId)
{

}