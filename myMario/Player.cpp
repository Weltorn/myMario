#include "Player.h"
#include "T_Scene.h"
Player::Player(LPCTSTR imgPath, int frameWidth, int frameHeight)
	:T_Sprite(imgPath, frameWidth, frameHeight)
{
	// ----- role status
	lifeCount = 3;
	isInEnvnt = false;
	eventId = -1;
	timer = GetTickCount();

	dir = DIR_RIGHT;		//初始化方向

	//------------------------------------------------------------初始化位置，暂不确定
	X;
	Y;


	// ----- MOVE STATUS
	bMove = false;		//是否水平移动状态
	bSquat = false;		//是否下蹲状态
	bJump = false;		//是否跳跃状态

	// -----JUMP STATUS
	jumpStatus =-1;	//跳跃状态0：上升，1：下降
	isBooting = false;	//是否跳跃加速状态

	// ----- MOVE--------------------------------------------------暂不确定，待初始化
	maxMoveSpeedX;
	maxRunSpeedX;
	currentMaxSpeed;
	friction;		//水平摩擦，控制惯性滑行距离

	// ----- JUMP--------------------------------------------------暂不确定，待初始化
	originJumpSpeedY;		//跳跃初始速度
	maxBootTime;		//最大加速时间（按住跳跃键的有效时间）
	gravity;			//基础重力加速度
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
			if (speedX > 0){		//惯性滑行状态，减速
				speedX -= friction;
			}
			else {				//恢复水平静止
				speedX = 0;
			}
		}	

		int ispeedX;
		//根据方向设置速度符号
		if (dir = DIR_LEFT)
			ispeedX = -abs(speedX);
		else if (dir = DIR_RIGHT)
			ispeedX = abs(speedX);

		X += ispeedX;
	}
}
//更新玩家纵坐标
void Player::updatePositionY()
{
	gravityEffect();
	Y = Y + speedY;
	if (speedY <= 0)		//速度小于零（向下），设置为下落状态
		jumpStatus = 1;
}
//开始跳跃
void Player::startJump()
{
	timer = GetTickCount();		//记录起跳时间，用于控制加速时间(isBooting)

	bJump = true;		//设置跳跃状态
	jumpStatus = 0;		//设置为上升阶段
	isBooting = true;
	speedY = originJumpSpeedY;	//设置跳跃初始速度
}
//落地
void Player::resetJump()
{
	bJump = false;			//设置跳跃状态
	jumpStatus = -1;		//非跳跃状态
	speedY = 0;				//设置竖直速度置零
	isBooting = false;		//加速状态结束

	stopMove(false);	//水平静止,有惯性
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
		currentGravity = gravity*2;		//重力增大，加快下落
	}
	speedY = (int)round(speedY - gravity);	//四舍五入
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
	
}
void Player::update()
{
	if (isBooting && timer + maxBootTime <= GetTickCount())	//跳跃加速时间结束
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
	speedX = 1;
	currentMaxSpeed = maxMoveSpeedX;
	bMove = true;
}

//按住shift键，提高加速上限
void Player::startSpeedup() {
	currentMaxSpeed = maxRunSpeedX;
}

//松开shift键，恢复加速上限
//若当前速度大于正常移动速度上限，将速度设置为正常移动上限
void Player::resetSpeedup() {
	currentMaxSpeed = maxMoveSpeedX;
	if (speedX > maxMoveSpeedX)
	{
		speedX = maxMoveSpeedX;
	}
}
//停止水平移动
void Player::stopMove(bool immediately) {
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

